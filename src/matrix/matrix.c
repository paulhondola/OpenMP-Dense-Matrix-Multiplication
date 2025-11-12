#include "matrix.h"
#include "../main/parameters.h"
#include <math.h>
#include <omp.h>

void matrix_create(Matrix *matrix, int size) {
  matrix->data = (double **)malloc(size * sizeof(double *));
  if (matrix->data == NULL) {
    printf("Error: Failed to allocate memory for matrix\n");
    exit(1);
  }
#pragma omp parallel for
  for (int i = 0; i < size; i++) {
    matrix->data[i] = (double *)malloc(size * sizeof(double));
    if (matrix->data[i] == NULL) {
      printf("Error: Failed to allocate memory for matrix\n");
      exit(1);
    }
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
#pragma omp parallel
  {
    unsigned int seed = SEED + omp_get_thread_num();
#pragma omp parallel for
    for (int i = 0; i < matrix.size; i++) {
      for (int j = 0; j < matrix.size; j++) {
        matrix.data[i][j] = UNIFORM_MIN + (UNIFORM_MAX - UNIFORM_MIN) *
                                              (double)rand_r(&seed) /
                                              (double)RAND_MAX;
      }
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
  double max_diff = 0.0;
#pragma omp parallel for reduction(max : max_diff)
  for (int i = 0; i < a.size; i++) {
    for (int j = 0; j < a.size; j++) {
      double diff = fabs(a.data[i][j] - b.data[i][j]);
      max_diff = (diff > max_diff) ? diff : max_diff;
    }
  }
  return max_diff <= EPSILON;
}
