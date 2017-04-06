#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
const int m_size = 1023;

MatrixAlgo *matrix_providers[] = {
    &NaiveMatrixProvider,
    &RowMajorMatrixProvider,
    &AVXMatrixProvider
};

int main()
{
    srand(time(NULL));
    float A[m_size][m_size];
    float B[m_size][m_size];
    float C[m_size][m_size];
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            A[i][j] = (float)(rand() % 100000);
            B[i][j] = (float)(rand() % 100000);
        }
    }

    MatrixAlgo *algo = matrix_providers[0];
    Matrix *m, *n, *ans;
    m = algo->create(m_size, m_size);
    n = algo->create(m_size, m_size);
    ans = algo->create(m_size, m_size);
    algo->mul(ans, m, n);

    for (int i = 1; i < 3; i++) {
        algo = matrix_providers[i];
        Matrix *dst;
        dst = algo->create(m_size, m_size);
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
