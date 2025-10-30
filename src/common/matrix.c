#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrix_create(Matrix *matrix, int rows, int cols) {
  matrix->data = (double **)malloc(rows * sizeof(double *));
  for (int i = 0; i < rows; i++) {
    matrix->data[i] = (double *)malloc(cols * sizeof(double));
  }
  matrix->rows = rows;
  matrix->cols = cols;
}

void matrix_fill_random(Matrix *matrix) {
  if (matrix->data == NULL) {
    return;
  }
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->cols; j++) {
      matrix->data[i][j] = UNIFORM_MIN + (UNIFORM_MAX - UNIFORM_MIN) *
                                             (double)rand() /
                                             (double)RAND_MAX;
    }
  }
}

void matrix_print(Matrix *matrix) {
  if (matrix->data == NULL) {
    return;
  }
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->cols; j++) {
      printf("%f ", matrix->data[i][j]);
    }
    printf("\n");
  }
}

void matrix_destroy(Matrix *matrix) {
  if (matrix->data == NULL) {
    return;
  }
  for (int i = 0; i < matrix->rows; i++) {
    free(matrix->data[i]);
  }
  free(matrix->data);
}