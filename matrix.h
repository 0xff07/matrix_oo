#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdbool.h>

/* predefined shortcut */
#define DECLARE_MATRIX(col, row) \
    typedef struct { float values[col][row]; } Mat ## col ## x ## row
DECLARE_MATRIX(3, 3);
DECLARE_MATRIX(4, 4);

typedef struct {
    int row, col;
    void *priv;
} Matrix;

typedef struct {
    Matrix *(*create)(int row, int col);
    void (*assign)(Matrix *thiz, float* that, int row, int col);
    bool (*equal)(const Matrix *l, const Matrix *r);
    bool (*mul)(Matrix *dst, const Matrix *l, const Matrix *r);
    void (*dump)(const Matrix *a);
} MatrixAlgo;

/* Available matrix providers */
extern MatrixAlgo NaiveMatrixProvider;

#endif /* MATRIX_H_ */
