// ABOUTME: Validation and benchmarking functions for matrix multiplication
// ABOUTME: Includes timing utilities and result comparison with epsilon tolerance

#ifndef TEST_H
#define TEST_H

#include "../common/matrix.h"

int test_serial_loop_permutations(Matrix a, Matrix b, double times[]);
int test_parallel_loop_permutations(Matrix a, Matrix b, int thread_count,
                                    int chunk, double times[]);
int test_classic_vs_improved(Matrix a, Matrix b, double times[], int chunk);
int test_tiled(Matrix a, Matrix b, double times[], int block_size);

#endif // TEST_H
