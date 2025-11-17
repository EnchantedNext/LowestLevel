#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  int rows;
  int cols;
  double **data;
} matrix_t;

void assignValueInMatrix(matrix_t *matrix, int row, int col, double val);

void multiplyMatrixByNumber(matrix_t *matrix, double val);

matrix_t *multiplyMatrices(matrix_t *mf, matrix_t *ms);

void stdoutMatrix(matrix_t *matrix);

void freeMatrix(matrix_t *matrix);

#endif
