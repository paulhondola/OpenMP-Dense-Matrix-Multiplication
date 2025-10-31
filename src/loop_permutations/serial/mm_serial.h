// ABOUTME: Serial matrix multiplication implementations for all six loop permutations
// ABOUTME: Baseline implementations without parallelization (i-j-k, i-k-j, j-i-k, j-k-i, k-i-j, k-j-i)

#ifndef MM_SERIAL_H
#define MM_SERIAL_H

#include "../../common/matrix.h"

typedef double (*serial_loop_permutation_function)(Matrix a, Matrix b, Matrix c);
extern serial_loop_permutation_function serial_f[PERMUTATIONS];

double serial_multiply_ijk(Matrix a, Matrix b, Matrix c);
double serial_multiply_ikj(Matrix a, Matrix b, Matrix c);
double serial_multiply_jik(Matrix a, Matrix b, Matrix c);
double serial_multiply_jki(Matrix a, Matrix b, Matrix c);
double serial_multiply_kij(Matrix a, Matrix b, Matrix c);
double serial_multiply_kji(Matrix a, Matrix b, Matrix c);

#endif // MM_SERIAL_H
