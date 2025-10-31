// ABOUTME: Matrix allocation, initialization, deallocation and helper functions
// ABOUTME: Random value generation and utilities used across all implementations

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 1
#define SEED 42
#define UNIFORM_MIN -100
#define UNIFORM_MAX 100
#define N 1000
#define PERMUTATIONS 6

typedef double** Matrix;

Matrix matrix_create(void);
void matrix_fill_random(Matrix matrix);
void matrix_fill_zero(Matrix matrix);
int validate(Matrix a, Matrix b);
void matrix_print(Matrix matrix);
void matrix_destroy(Matrix matrix);

#endif // MATRIX_H
