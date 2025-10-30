// ABOUTME: Matrix allocation, initialization, deallocation and helper functions
// ABOUTME: Random value generation and utilities used across all implementations

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

#define DEBUG 1
#define SEED 42
#define UNIFORM_MIN -100
#define UNIFORM_MAX 100

typedef struct {
  double **data;
  int rows;
  int cols;
} Matrix;

void matrix_create(Matrix *matrix, int rows, int cols);
void matrix_fill_random(Matrix *matrix);
void matrix_fill_zero(Matrix *matrix);
void matrix_print(Matrix *matrix);
void matrix_destroy(Matrix *matrix);

#endif // MATRIX_H
