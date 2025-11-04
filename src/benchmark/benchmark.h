// ABOUTME: Validation and benchmarking functions for matrix multiplication
// ABOUTME: Includes timing utilities and result comparison with epsilon tolerance

#ifndef TEST_H
#define TEST_H

#include "../common/matrix.h"
#define CLASSIC_VS_IMPROVED_TESTS 8
#define LOOP_PERMUTATIONS 6
#define TILED_TESTS 4

void compute_speedup(double time_results[], double speedup_results[], int size);
int test_serial_loop_permutations(double time_results[], Matrix a, Matrix b);
int test_parallel_loop_permutations(double time_results[], Matrix a, Matrix b, int thread_count,
                                    int chunk);
int test_classic_vs_improved(double time_results[], Matrix a, Matrix b, int chunk);
int test_tiled(double time_results[], Matrix a, Matrix b, int thread_count,
               int block_size);

#endif // TEST_H
