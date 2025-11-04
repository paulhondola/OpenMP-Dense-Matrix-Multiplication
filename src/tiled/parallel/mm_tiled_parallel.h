// ABOUTME: OpenMP parallel cache-blocked matrix multiplication implementation
// ABOUTME: Combines tiled algorithm with parallelization for optimal performance

#ifndef MM_TILED_PARALLEL_H
#define MM_TILED_PARALLEL_H

#include "../../common/matrix.h"

double parallel_multiply_tiled(Matrix a, Matrix b, Matrix c, int thread_count,
                               int chunk_size, int block_size);
#endif // MM_TILED_PARALLEL_H
