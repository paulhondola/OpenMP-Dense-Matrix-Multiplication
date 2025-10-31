#include "test.h"
#include "../common/matrix.h"
#include "../loop_permutations/parallel/mm_parallel.h"
#include "../loop_permutations/serial/mm_serial.h"
#define NUM_PERMUTATIONS 6

int run_serial_loop_permutation(Matrix a, Matrix b, Matrix reference,
                                int permutation) {
  Matrix c = matrix_create();
  switch (permutation) {
  case 1:
    serial_multiply_ikj(a, b, c);
    break;
  case 2:
    serial_multiply_jik(a, b, c);
    break;
  case 3:
    serial_multiply_jki(a, b, c);
    break;
  case 4:
    serial_multiply_kij(a, b, c);
    break;
  case 5:
    serial_multiply_kji(a, b, c);
    break;
  default:
    return 0;
  }

  int result = validate(reference, c);
  matrix_destroy(c);

#ifdef DEBUG
  if (result) {
    printf("Permutation %d is correct\n", permutation);
  } else {
    printf("Permutation %d is incorrect\n", permutation);
  }
  printf("------------------------------------------\n");
#endif

  return result;
}

int test_serial_loop_permutations(Matrix a, Matrix b) {

#ifdef DEBUG
  printf("Serial - Testing loop "
         "permutations\n------------------------------------------\n");
#endif

  Matrix reference = matrix_create();
  serial_multiply_ijk(a, b, reference);

  int correct_count = 0;
  for (int i = 1; i < NUM_PERMUTATIONS; i++) {
    if (run_serial_loop_permutation(a, b, reference, i)) {
      correct_count++;
    }
  }

  matrix_destroy(reference);

#ifdef DEBUG
  printf("Serial - Test completed\n");
  printf("Correct count: %d\n", correct_count);
  printf("Total permutations: %d\n", NUM_PERMUTATIONS - 1);
  printf("------------------------------------------\n");
#endif

  return correct_count == NUM_PERMUTATIONS - 1;
}

int run_parallel_loop_permutation(Matrix a, Matrix b, Matrix reference,
                                  int permutation, int thread_count,
                                  int chunk) {
  Matrix c = matrix_create();
  switch (permutation) {
  case 1:
    parallel_multiply_ikj(a, b, c, thread_count, chunk);
    break;
  case 2:
    parallel_multiply_jik(a, b, c, thread_count, chunk);
    break;
  case 3:
    parallel_multiply_jki(a, b, c, thread_count, chunk);
    break;
  case 4:
    parallel_multiply_kij(a, b, c, thread_count, chunk);
    break;
  case 5:
    parallel_multiply_kji(a, b, c, thread_count, chunk);
    break;
  default:
    return 0;
  }

  int result = validate(reference, c);
  matrix_destroy(c);

#ifdef DEBUG
  if (result) {
    printf("Permutation %d is correct\n", permutation);
  } else {
    printf("Permutation %d is incorrect\n", permutation);
  }
  printf("------------------------------------------\n");
#endif

  return result;
}

int test_parallel_loop_permutations(Matrix a, Matrix b, int thread_count,

                                    int chunk) {
  // set the IJK permutation as reference
  Matrix reference = matrix_create();
  parallel_multiply_ijk(a, b, reference, thread_count, chunk);

  int correct_count = 0;
  for (int i = 1; i < NUM_PERMUTATIONS; i++) {
    if (run_parallel_loop_permutation(a, b, reference, i, thread_count,
                                      chunk)) {
      correct_count++;
    }
  }

  matrix_destroy(reference);

#ifdef DEBUG
  printf("Parallel - Test completed\n");
  printf("Correct count: %d\n", correct_count);
  printf("Total permutations: %d\n", NUM_PERMUTATIONS - 1);
  printf("------------------------------------------\n");
#endif

  return correct_count == NUM_PERMUTATIONS - 1;
}