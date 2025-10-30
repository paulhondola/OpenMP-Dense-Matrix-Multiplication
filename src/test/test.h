// ABOUTME: Validation and benchmarking functions for matrix multiplication
// ABOUTME: Includes timing utilities and result comparison with epsilon tolerance

#ifndef TEST_H
#define TEST_H

#include "../common/matrix.h"


#define EPSILON 1e-6

int validate(Matrix *a, Matrix *b);
int test_loop_permutations(Matrix *a, Matrix *b);

#endif // TEST_H
