// ABOUTME: Matrix allocation, initialization, deallocation and helper functions
// ABOUTME: Random value generation and utilities used across all implementations

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  double** data;
  int size;
} Matrix;

typedef double (*serial_loop_benchmark)(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c);
typedef double (*parallel_loop_benchmark)(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c, int thread_count, int chunk_size);

void matrix_create(Matrix* matrix, int size);
void matrix_fill_random(Matrix *restrict matrix);
void matrix_fill_zero(Matrix *restrict matrix);
int validate(const Matrix *restrict a, const Matrix *restrict b);
void matrix_print(const Matrix *restrict matrix);
void matrix_destroy(Matrix *restrict matrix);

#endif // MATRIX_H
