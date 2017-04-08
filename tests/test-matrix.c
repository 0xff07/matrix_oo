#include "matrix.h"
#include "stopwatch.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define m_row_size 1025
#define col_row_size 1025
#define n_col_size 1025
#define range 100000

MatrixAlgo *matrix_providers[] = {
    &NaiveMatrixProvider,
    &RowMajorMatrixProvider,
    &AVXMatrixProvider
};

int main()
{
    watch_p watch = Stopwatch.create();
    double mul_time[3] = {0};

    /* use naive algorithm as answer */
    MatrixAlgo *algo = matrix_providers[0];
    Matrix *m, *n, *ans;
    m = algo->create(m_row_size, col_row_size);
    n = algo->create(col_row_size, n_col_size);
    algo->fill(m, range);
    algo->fill(n, range);
    ans = algo->create(m_row_size, n_col_size);

    /* evaluate time of naive mul */
    Stopwatch.start(watch);
    algo->mul(ans, m, n);
    Stopwatch.stop(watch);
    mul_time[0] += Stopwatch.read(watch);
    Stopwatch.reset(watch);

    /* test different version */
    for (int i = 1; i < 3; i++) {
        algo = matrix_providers[i];
        Matrix *dst;
        dst = algo->create(m_row_size, n_col_size);

        Stopwatch.start(watch);
        algo->mul(dst, m, n);
        Stopwatch.stop(watch);
        mul_time[i] += Stopwatch.read(watch);
        Stopwatch.reset(watch);

        if(!(algo->equal(dst, ans))) {
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
    printf("%5.3f %5.3f %5.3f\n", mul_time[0], mul_time[1], mul_time[2]);
    algo->free(m);
    algo->free(n);
    algo->free(ans);
}
