// ABOUTME: Cache-blocked serial matrix multiplication implementation
// ABOUTME: Tiled algorithm for improved cache locality without parallelization

#ifndef MM_TILED_SERIAL_H
#define MM_TILED_SERIAL_H

#include "../../matrix/matrix.h"

double serial_multiply_tiled(Matrix a, Matrix b, Matrix c, int block_size);

#endif // MM_TILED_SERIAL_H
