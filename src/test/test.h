// ABOUTME: Validation and benchmarking functions for matrix multiplication
// ABOUTME: Includes timing utilities and result comparison with epsilon tolerance

#ifndef TEST_H
#define TEST_H

#include "../common/matrix.h"
#include <stdbool.h>

#define EPSILON 1e-6

bool validate(Matrix *matrix1, Matrix *matrix2);

#endif // TEST_H
