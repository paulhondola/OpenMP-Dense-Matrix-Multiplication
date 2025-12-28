// ABOUTME: Serial matrix multiplication implementations for all six loop permutations
// ABOUTME: Baseline implementations without parallelization (i-j-k, i-k-j, j-i-k, j-k-i, k-i-j, k-j-i)

#ifndef MM_SERIAL_H
#define MM_SERIAL_H

#include "../../matrix/matrix.h"

extern serial_loop_benchmark serial_loop_benchmark_functions[];

double serial_multiply_ijk(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c);
double serial_multiply_ikj(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c);
double serial_multiply_jik(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c);
double serial_multiply_jki(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c);
double serial_multiply_kij(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c);
double serial_multiply_kji(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c);

#endif // MM_SERIAL_H
