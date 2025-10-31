#include "matrix.h"
#include <math.h>

#define EPSILON 1e-9

void matrix_create(Matrix *matrix, int size) {
  matrix->data = (double **)malloc(size * sizeof(double *));
#pragma omp parallel for
  for (int i = 0; i < size; i++) {
    matrix->data[i] = (double *)malloc(size * sizeof(double));
  }
  matrix->size = size;
}

void matrix_fill_zero(Matrix matrix) {
#pragma omp parallel for
  for (int i = 0; i < matrix.size; i++) {
    for (int j = 0; j < matrix.size; j++) {
      matrix.data[i][j] = 0;
    }
  }
}

void matrix_fill_random(Matrix matrix) {
#pragma omp parallel for
  for (int i = 0; i < matrix.size; i++) {
    for (int j = 0; j < matrix.size; j++) {
      matrix.data[i][j] = UNIFORM_MIN + (UNIFORM_MAX - UNIFORM_MIN) *
                                            (double)rand() / (double)RAND_MAX;
    }
  }
}

void matrix_print(Matrix matrix) {
  for (int i = 0; i < matrix.size; i++) {
    for (int j = 0; j < matrix.size; j++) {
      printf("%f ", matrix.data[i][j]);
    }
    printf("\n");
  }
}

void matrix_destroy(Matrix matrix) {
  for (int i = 0; i < matrix.size; i++) {
    free(matrix.data[i]);
  }
  free(matrix.data);
}

int validate(Matrix a, Matrix b) {
  int result = 1;
#pragma omp parallel for reduction(&& : result)
  for (int i = 0; i < a.size; i++) {
    for (int j = 0; j < a.size; j++) {
      if (fabs(a.data[i][j] - b.data[i][j]) > EPSILON) {
        result = 0;
      }
    }
  }
  return result;
}
