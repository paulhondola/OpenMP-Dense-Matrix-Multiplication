#include "../common/matrix.h"
#include "../test/test.h"
#include <omp.h>

int main(int argc, char *argv[]) {

  Matrix a, b;
  a = matrix_create();
  b = matrix_create();
  matrix_fill_random(a);
  matrix_fill_random(b);

  if (test_serial_loop_permutations(a, b)) {
    printf("All implementations are correct\n");
  } else {
    printf("Some implementations are incorrect\n");
  }

  if (test_parallel_loop_permutations(a, b, omp_get_num_threads(), 1)) {
    printf("All implementations are correct\n");
  } else {
    printf("Some implementations are incorrect\n");
  }

  matrix_destroy(a);
  matrix_destroy(b);

  return 0;
}
