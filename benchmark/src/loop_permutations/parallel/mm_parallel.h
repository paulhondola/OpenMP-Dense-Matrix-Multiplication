// ABOUTME: OpenMP parallel matrix multiplication implementations for all six loop permutations
// ABOUTME: Parallelized versions using #pragma omp for (i-j-k, i-k-j, j-i-k, j-k-i, k-i-j, k-j-i)

#ifndef MM_PARALLEL_H
#define MM_PARALLEL_H

#include "../../matrix/matrix.h"

extern parallel_loop_benchmark parallel_loop_benchmark_functions[];

double parallel_multiply_ijk(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c, int thread_count,
                             int chunk);
double parallel_multiply_ikj(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c, int thread_count,
                             int chunk);
double parallel_multiply_jik(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c, int thread_count,
                             int chunk);
double parallel_multiply_jki(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c, int thread_count,
                             int chunk);
double parallel_multiply_kij(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c, int thread_count,
                             int chunk);
double parallel_multiply_kji(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c, int thread_count,
                             int chunk);

#endif // MM_PARALLEL_H
