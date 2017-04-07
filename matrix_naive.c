#include "matrix.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <immintrin.h>
#include <string.h>
#include <malloc.h>

#define PRIV(x) \
    ((float**) ((x)->priv))

/* malloc and make float **ret can use ret[i][j] to access data. */
static float **matrix_alloc(int row, int col)
{
    int idx_size = row * sizeof(float*);
    int data_size = row * col * sizeof(float*);
    void* ret = memalign(32, idx_size + data_size);
    memset(ret, 0, idx_size + data_size);

    float** idx = (float**)ret;
    float* data = (float*)((char*)ret + idx_size);

    for(int i = 0; i < row; i++)
        idx[i] = data + col * i;

    return ret;
}

static Matrix *create(int row, int col)
{
    Matrix *ret = (Matrix*)malloc(sizeof(Matrix));
    ret->row = row;
    ret->col = col;
    ret -> priv = matrix_alloc(row, col);
    return ret;
}

static void matrix_free(Matrix *thiz)
{
    if(!thiz) return;
    free(thiz->priv);
    free(thiz);
}

static void assign(Matrix *thiz, float* that, int row, int col)
{
    /* FIXME: don't hardcode row & col */
    assert(thiz->col == col && thiz->row == row);

    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            PRIV(thiz)[i][j] = that[i*col + j];
}

static const float epsilon = 1 / 10000.0;

static bool equal(const Matrix *l, const Matrix *r)
{
    if(l->col != r->col || l->row != r->row)
        return false;

    int m = l->row;
    int n = l->col;

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (PRIV(l)[i][j] + epsilon < PRIV(r)[i][j] ||
                    PRIV(r)[i][j] + epsilon < PRIV(l)[i][j])
                return false;
    return true;
}

static bool naive_mul(Matrix *dst, const Matrix *l, const Matrix *r)
{
    /* FIXME: error hanlding */
    int l_row = l->row, l_col = l->col;
    int r_row = r->row, r_col = r->col;

    assert(l_col == r->row);

    dst->priv = matrix_alloc(l_row, r_col);
    for (int i = 0; i < l_row; i++)
        for (int j = 0; j < r_col; j++)
            for (int k = 0; k < l_col; k++)
                PRIV(dst)[i][j] += PRIV(l)[i][k] * PRIV(r)[k][j];
    return true;
}

static bool row_major_mul(Matrix *dst, const Matrix *l, const Matrix *r)
{
    /* FIXME: error hanlding */
    int l_row = l->row, l_col = l->col;
    int r_row = r->row, r_col = r->col;

    assert(l_col == r->row);

    dst->priv = matrix_alloc(l_row, r_col);
    for (int i = 0; i < l_row; i++) {
        for (int j = 0; j < l_col; j++) {
            float coeff = PRIV(l)[i][j];
            for (int k = 0; k < r_col; k++)
                PRIV(dst)[i][k] += coeff * PRIV(r)[j][k];
        }
    }
    return true;
}


static bool avx_mul(Matrix *dst, const Matrix *l, const Matrix *r)
{
    /* FIXME: error hanlding */
    int l_row = l->row, l_col = l->col;
    int r_row = r->row, r_col = r->col;

    assert(l_col == r->row);

    free(dst->priv);
    dst->priv = matrix_alloc(l_row, r_col);
    for (int i = 0; i < l_row; i++) {
        for (int j = 0; j < l_col; j++) {
            float coeff = PRIV(l)[i][j];
            int lower = r_col & 7;
            int upper = r_col - lower;
            for (int k = 0; k + 8 <= upper; k+=8) {
                __m256 ymm0;
                __m256 ymm1;
                __m256 ymm2;
                __m256 ymm3;
                ymm0 = _mm256_loadu_ps(&(PRIV(r)[j][k]));
                ymm1 = _mm256_set1_ps(coeff);
                ymm2 = _mm256_loadu_ps(&(PRIV(dst)[i][k]));
                ymm3 = _mm256_fmadd_ps(ymm0, ymm1, ymm2);
                _mm256_storeu_ps((float *)(&(PRIV(dst)[i][k])), ymm3);

            }
            for (int k = upper; k < r_col; k++) {
                PRIV(dst)[i][k] += coeff * PRIV(r)[j][k];
            }
        }
    }
    return true;
}

static void dump(const Matrix *a)
{
    int row = a->row;
    int col = a->col;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++)
            printf("%6.3f  ", ((float**)(a->priv))[i][j]);
        printf("\n");
    }
    printf("\n");
}

MatrixAlgo NaiveMatrixProvider = {
    .create = create,
    .assign = assign,
    .equal = equal,
    .mul = naive_mul,
    .dump = dump,
    .free = matrix_free
};

MatrixAlgo RowMajorMatrixProvider = {
    .create = create,
    .assign = assign,
    .equal = equal,
    .mul = row_major_mul,
    .dump = dump,
    .free = matrix_free
};

MatrixAlgo AVXMatrixProvider = {
    .create = create,
    .assign = assign,
    .equal = equal,
    .mul = avx_mul,
    .dump = dump,
    .free = matrix_free
};
