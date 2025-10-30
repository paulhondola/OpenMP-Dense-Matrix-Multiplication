#include "../common/matrix.h"
#include "../loop_permutations/serial/mm_serial.h"
#include "../test/test.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000

int main(int argc, char *argv[]) {
  Matrix a, b;
  Matrix c_ijk, c_ikj;
  matrix_create(&a, N, N);
  matrix_create(&b, N, N);
  matrix_create(&c_ijk, N, N);
  matrix_create(&c_ikj, N, N);

  matrix_fill_random(&a);
  matrix_fill_random(&b);

  serial_multiply_ijk(&a, &b, &c_ijk);
  serial_multiply_ikj(&a, &b, &c_ikj);

  if (validate(&c_ijk, &c_ikj)) {
    printf("All implementations are correct\n");
  } else {
    printf("Some implementations are incorrect\n");
  }

  matrix_destroy(&a);
  matrix_destroy(&b);
  matrix_destroy(&c_ijk);
  matrix_destroy(&c_ikj);

  return 0;
}