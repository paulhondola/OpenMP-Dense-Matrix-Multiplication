// ABOUTME: Cache-blocked serial matrix multiplication implementation
// ABOUTME: Tiled algorithm for improved cache locality without parallelization

#ifndef MM_TILED_SERIAL_H
#define MM_TILED_SERIAL_H

#include "../../matrix/matrix.h"

double serial_multiply_tiled(const Matrix *restrict a, const Matrix *restrict b, Matrix *restrict c, int block_size);

#endif // MM_TILED_SERIAL_H
