// ABOUTME: OpenMP parallel matrix multiplication implementations for all six loop permutations
// ABOUTME: Parallelized versions using #pragma omp for (i-j-k, i-k-j, j-i-k, j-k-i, k-i-j, k-j-i)

#ifndef MM_PARALLEL_H
#define MM_PARALLEL_H

#include "../../common/matrix.h"

double parallel_multiply_ijk(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk);
double parallel_multiply_ikj(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk);
double parallel_multiply_jik(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk);
double parallel_multiply_jki(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk);
double parallel_multiply_kij(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk);
double parallel_multiply_kji(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk);

#endif // MM_PARALLEL_H
