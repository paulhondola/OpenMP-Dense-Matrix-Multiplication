// ABOUTME: Validation and benchmarking functions for matrix multiplication
// ABOUTME: Includes timing utilities and result comparison with epsilon tolerance

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "../matrix/matrix.h"

void compute_speedup(double time_results[], double speedup_results[], int size);
void test_serial_loop_permutations(double time_results[], const Matrix *restrict a, const Matrix *restrict b);
void test_parallel_loop_permutations(double time_results[], const Matrix *restrict a, const Matrix *restrict b, int thread_count,
                                    int chunk);
void test_serial_parallel_scaling_classic(double time_results[], const Matrix *restrict a, const Matrix *restrict b, int chunk_size);
void test_serial_parallel_scaling_improved(double time_results[], const Matrix *restrict a, const Matrix *restrict b, int chunk_size);
void test_tiled(double time_results[], const Matrix *restrict a, const Matrix *restrict b, int thread_count,
                int block_size);

#endif // BENCHMARK_H
