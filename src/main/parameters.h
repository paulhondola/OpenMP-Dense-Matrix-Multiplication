#ifndef PARAMETERS_H
#define PARAMETERS_H

// debug parameters
#define DEBUG
//#define DEBUG_MATRIX

// random number generation parameters
#define SEED 42
#define UNIFORM_MIN -10
#define UNIFORM_MAX 10

// benchmark parameters
#define MATRIX_SIZES {480, 640, 960, 1280, 1920}
#define THREAD_COUNT 10
#define CHUNK_SIZES {48, 96, 192}
#define BLOCK_SIZES {48, 96, 128}

// number of tests for each benchmark
#define CLASSIC_VS_IMPROVED_TESTS 4
#define LOOP_PERMUTATIONS 6
#define TILED_TESTS 5

// test colors
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

// threshold value for validation
#define EPSILON 1e-6

#endif // PARAMETERS_H
