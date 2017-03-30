#include "matrix.h"
#include <stdio.h>

MatrixAlgo *matrix_providers[] = {
    &NaiveMatrixProvider,
};

int main()
{
    MatrixAlgo *algo = matrix_providers[0];

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
        return 0;
    return -1;
}
