// ABOUTME: OpenMP parallel cache-blocked matrix multiplication implementation
// ABOUTME: Combines tiled algorithm with parallelization for optimal performance

#ifndef MM_TILED_PARALLEL_H
#define MM_TILED_PARALLEL_H

#include "../../matrix/matrix.h"

double parallel_multiply_tiled(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c, int thread_count,
                               int block_size);

double parallel_multiply_tiled_tasks(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c,
                                     int thread_count, int block_size);
#endif // MM_TILED_PARALLEL_H
