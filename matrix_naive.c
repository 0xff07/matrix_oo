#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>


#define PRIV(x) \
    ((float**) ((x)->priv))

/* malloc and make float **ret can use ret[i][j] to access data. */
static float** matrix_alloc(int row, int col)
{
    int idx_size = row * sizeof(float*);
    int data_size = row * col * sizeof(float*);
    void* ret = calloc(idx_size + data_size, sizeof(char));

    float** idx = (float**)ret;
    float* data = (float*)((char*)ret + idx_size);

    for(int i = 0; i < row; i++)
        idx[i] = data + col * i;

    return ret;
}

static Matrix* create(int row, int col)
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

bool mul(Matrix *dst, const Matrix *l, const Matrix *r)
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

bool row_major_mul(Matrix *dst, const Matrix *l, const Matrix *r)
{
    /* FIXME: error hanlding */
    int l_row = l->row, l_col = l->col;
    int r_row = r->row, r_col = r->col;

    assert(l_col == r->row);

    dst->priv = matrix_alloc(l_row, r_col);
    for (int i = 0; i < l_col; i++) {
        for (int j = 0; j < l_row; j++) {
            float coeff = PRIV(l)[i][j];
            for (int k = 0; k < r_col; k++)
                PRIV(dst)[i][k] += coeff * PRIV(r)[j][k];
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
            printf("%6.3f", ((float**)(a->priv))[i][j]);
        printf("\n");
    }
    printf("\n");
}

MatrixAlgo NaiveMatrixProvider = {
    .create = create,
    .assign = assign,
    .equal = equal,
    .mul = mul,
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
