#include "matrix.h"
#include <math.h>

#define EPSILON 1e-9

Matrix matrix_create(void) {
  Matrix matrix = (double **)malloc(N * sizeof(double *));
#pragma omp parallel for
  for (int i = 0; i < N; i++) {
    matrix[i] = (double *)malloc(N * sizeof(double));
  }
  return matrix;
}

void matrix_fill_zero(Matrix matrix) {
#pragma omp parallel for
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = 0;
    }
  }
}

void matrix_fill_random(Matrix matrix) {
#pragma omp parallel for
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = UNIFORM_MIN + (UNIFORM_MAX - UNIFORM_MIN) *
                                       (double)rand() / (double)RAND_MAX;
    }
  }
}

void matrix_print(Matrix matrix) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%f ", matrix[i][j]);
    }
    printf("\n");
  }
}

void matrix_destroy(Matrix matrix) {
  for (int i = 0; i < N; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

int validate(Matrix a, Matrix b) {
  int result = 1;
#pragma omp parallel for reduction(&& : result)
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (fabs(a[i][j] - b[i][j]) > EPSILON) {
        result = 0;
      }
    }
  }
  return result;
}
