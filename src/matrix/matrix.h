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

typedef double (*serial_loop_benchmark)(Matrix a, Matrix b, Matrix c);
typedef double (*parallel_loop_benchmark)(Matrix a, Matrix b, Matrix c, int thread_count, int chunk_size);

void matrix_create(Matrix* matrix, int size);
void matrix_fill_random(Matrix matrix);
void matrix_fill_zero(Matrix matrix);
int validate(Matrix a, Matrix b);
void matrix_print(Matrix matrix);
void matrix_destroy(Matrix matrix);

#endif // MATRIX_H
