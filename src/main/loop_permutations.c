#include "../common/matrix.h"
#include "../test/test.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000

int main(int argc, char *argv[]) {

  Matrix a, b;
  matrix_create(&a, N, N);
  matrix_create(&b, N, N);
  matrix_fill_random(&a);
  matrix_fill_random(&b);

  if (test_loop_permutations(&a, &b)) {
    printf("All implementations are correct\n");
  } else {
    printf("Some implementations are incorrect\n");
  }

  matrix_destroy(&a);
  matrix_destroy(&b);

  return 0;
}