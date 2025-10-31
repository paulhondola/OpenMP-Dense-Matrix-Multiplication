#include "../common/matrix.h"
#include "../test/test.h"
#include <omp.h>

int main(int argc, char *argv[]) {

  Matrix a, b;
  a = matrix_create();
  b = matrix_create();
  matrix_fill_random(a);
  matrix_fill_random(b);

  test_serial_loop_permutations(a, b);
  test_parallel_loop_permutations(a, b, omp_get_num_threads(), 1);

  matrix_destroy(a);
  matrix_destroy(b);

  return 0;
}
