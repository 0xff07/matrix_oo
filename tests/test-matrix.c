#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
const int m_row_size = 100;
const int col_row_size = 99;
const int n_col_size = 17;

MatrixAlgo *matrix_providers[] = {
    &NaiveMatrixProvider,
    &RowMajorMatrixProvider,
    &AVXMatrixProvider
};

int main()
{
    srand(time(NULL));
    float A[m_row_size][col_row_size];
    float B[col_row_size][n_col_size];

    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < col_row_size; j++)
            A[i][j] = (float)(rand() % 100);

    for (int i = 0; i < col_row_size; i++)
        for(int j = 0; j < n_col_size; j++)
            B[i][j] = (float)(rand() % 100);

    MatrixAlgo *algo = matrix_providers[0];
    Matrix *m, *n, *ans;
    m = algo->create(m_row_size, col_row_size);
    n = algo->create(col_row_size, n_col_size);
    algo->assign(m, &A[0][0], m_row_size, col_row_size);
    algo->assign(n, &B[0][0], col_row_size, n_col_size);
    ans = algo->create(m_row_size, n_col_size);
    algo->mul(ans, m, n);

    for (int i = 1; i < 3; i++) {
        algo = matrix_providers[i];
        Matrix *dst;
        dst = algo->create(m_row_size, n_col_size);
        algo->mul(dst, m, n);

        if (algo->equal(dst, ans))
            printf("%dth matrix provider succeeded.\n", i);
        else {
            printf("%dth matrix provider failed.\n", i);
            printf("A : \n");
            algo->dump(m);
            printf("B : \n");
            algo->dump(n);
            printf("expect : \n");
            algo->dump(ans);
            printf("actual : \n");
            algo->dump(dst);
        }
        algo->free(dst);
    }
    algo->free(m);
    algo->free(n);
    algo->free(ans);
}
