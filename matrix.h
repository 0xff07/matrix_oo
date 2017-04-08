#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdbool.h>

typedef struct {
    int row, col;
    void *priv;
} Matrix;

typedef struct {
    Matrix *(*create)(int row, int col);
    void (*assign)(Matrix *thiz, float* that, int row, int col);
    bool (*equal)(const Matrix *l, const Matrix *r);
    bool (*mul)(Matrix *dst, const Matrix *l, const Matrix *r);
    void (*fill)(Matrix *thiz, int range);
    void (*dump)(const Matrix *a);
    void (*free) (Matrix *thiz);
} MatrixAlgo;

/* Available matrix providers */
extern MatrixAlgo NaiveMatrixProvider;
extern MatrixAlgo RowMajorMatrixProvider;
extern MatrixAlgo AVXMatrixProvider;

#endif /* MATRIX_H_ */
