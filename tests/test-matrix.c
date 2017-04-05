#include "matrix.h"
#include <stdio.h>

MatrixAlgo *matrix_providers[] = {
    &NaiveMatrixProvider,
    &RowMajorMatrixProvider,
    &AVXMatrixProvider
};

int main()
{
    float A[4][4] = {
        { 1, 2, 3, 4, },
        { 5, 6, 7, 8, },
        { 1, 2, 3, 4, },
        { 5, 6, 7, 8, },
    };

    float B[4][4] = {
        { 1, 2, 3, 4, },
        { 5, 6, 7, 8, },
        { 1, 2, 3, 4, },
        { 5, 6, 7, 8, },
    };

    float C[4][4] = {
        { 34,  44,  54,  64, },
        { 82, 108, 134, 160, },
        { 34,  44,  54,  64, },
        { 82, 108, 134, 160, },
    };

    MatrixAlgo *algo;
    for (int i = 0; i < 3; i++) {
        algo = matrix_providers[i];
        Matrix *dst, *m, *n, *fixed;
        dst = algo->create(4, 4);
        m = algo->create(4, 4);
        n = algo->create(4, 4);
        fixed = algo->create(4, 4);

        algo->assign(m, &A[0][0], 4, 4);
        algo->assign(n, &B[0][0], 4, 4);
        algo->assign(fixed, &C[0][0], 4, 4);

        algo->mul(dst, m, n);

        if (algo->equal(dst, fixed))
            printf("%dth matrix provider succeeded.\n", i);
        else {
            printf("%dth matrix provider failed.\n", i);
            printf("expect : \n");
            algo->dump(fixed);
            printf("actuel : \n");
            algo->dump(dst);
        }
        algo->free(dst);
        algo->free(m);
        algo->free(n);
        algo->free(fixed);
    }
}
