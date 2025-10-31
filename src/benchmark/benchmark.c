#include "benchmark.h"
#include "../common/matrix.h"
#include "../loop_permutations/parallel/mm_parallel.h"
#include "../loop_permutations/serial/mm_serial.h"

int run_serial_loop_permutation(Matrix a, Matrix b, Matrix reference,
                                int permutation, double times[]) {

  Matrix c = matrix_create();
  times[permutation] = serial_f[permutation](a, b, c);
  int result = validate(reference, c);
  matrix_destroy(c);

#ifdef DEBUG
  if (result) {
    printf("Permutation %d is correct\n", permutation);
  } else {
    printf("Permutation %d is incorrect\n", permutation);
  }
  printf("Time: %f seconds\n", times[permutation]);
  printf("------------------------------------------\n");
#endif

  return result;
}

int test_serial_loop_permutations(Matrix a, Matrix b, double times[]) {

#ifdef DEBUG
  printf("Serial - Testing loop "
         "permutations\n------------------------------------------\n");
#endif

  Matrix reference = matrix_create();
  times[0] = serial_f[0](a, b, reference);

#ifdef DEBUG
  printf("Time: %f seconds\n", times[0]);
  printf("------------------------------------------\n");
#endif

  int correct_count = 0;
  for (int i = 1; i < PERMUTATIONS; i++) {
    if (run_serial_loop_permutation(a, b, reference, i, times)) {
      correct_count++;
    }
  }

  matrix_destroy(reference);

#ifdef DEBUG
  printf("Serial - Test completed\n");
  printf("Correct count: %d\n", correct_count);
  printf("Total permutations: %d\n", PERMUTATIONS - 1);
  printf("------------------------------------------\n");
#endif

  return correct_count == PERMUTATIONS - 1;
}

int run_parallel_loop_permutation(Matrix a, Matrix b, Matrix reference,
                                  int permutation, int thread_count, int chunk,
                                  double times[]) {

  Matrix c = matrix_create();
  times[permutation] = parallel_f[permutation](a, b, c, thread_count, chunk);
  int result = validate(reference, c);
  matrix_destroy(c);

#ifdef DEBUG
  if (result) {
    printf("Permutation %d is correct\n", permutation);
  } else {
    printf("Permutation %d is incorrect\n", permutation);
  }
  printf("Time: %f seconds\n", times[permutation]);
  printf("------------------------------------------\n");
#endif

  return result;
}

int test_parallel_loop_permutations(Matrix a, Matrix b, int thread_count,
                                    int chunk, double times[]) {

#ifdef DEBUG
  printf("Parallel - Testing loop "
         "permutations\n------------------------------------------\n");
#endif

  // set the IJK permutation as reference
  Matrix reference = matrix_create();
  times[0] = parallel_f[0](a, b, reference, thread_count, chunk);

#ifdef DEBUG
  printf("Time: %f seconds\n", times[0]);
  printf("------------------------------------------\n");
#endif

  int correct_count = 0;
  for (int i = 1; i < PERMUTATIONS; i++) {
    if (run_parallel_loop_permutation(a, b, reference, i, thread_count, chunk,
                                      times)) {
      correct_count++;
    }
  }

  matrix_destroy(reference);

#ifdef DEBUG
  printf("Parallel - Test completed\n");
  printf("Correct count: %d\n", correct_count);
  printf("Total permutations: %d\n", PERMUTATIONS - 1);
  printf("------------------------------------------\n");
#endif

  return correct_count == PERMUTATIONS - 1;
}

int test_classic_vs_improved(Matrix a, Matrix b, double times[], int chunk) {

  Matrix c = matrix_create();
  times[0] = serial_multiply_ijk(a, b, c);
  times[1] = parallel_multiply_ijk(a, b, c, 2, chunk);
  times[2] = parallel_multiply_ijk(a, b, c, 4, chunk);
  times[3] = parallel_multiply_ijk(a, b, c, 8, chunk);

  times[4] = serial_multiply_ikj(a, b, c);
  times[5] = parallel_multiply_ikj(a, b, c, 2, chunk);
  times[6] = parallel_multiply_ikj(a, b, c, 4, chunk);
  times[7] = parallel_multiply_ikj(a, b, c, 8, chunk);

  matrix_destroy(c);

#ifdef DEBUG
  printf("Classic vs Improved - Test completed\n");
  printf("------------------------------------------\n");
  printf("SERIAL: %f seconds\n", times[0]);
  printf("PARALLEL 2 THREADS: %f seconds\n", times[1]);
  printf("PARALLEL 4 THREADS: %f seconds\n", times[2]);
  printf("PARALLEL 8 THREADS: %f seconds\n", times[3]);
  printf("IMPROVED SERIAL: %f seconds\n", times[4]);
  printf("IMPROVED PARALLEL 2 THREADS: %f seconds\n", times[5]);
  printf("IMPROVED PARALLEL 4 THREADS: %f seconds\n", times[6]);
  printf("IMPROVED PARALLEL 8 THREADS: %f seconds\n", times[7]);
  printf("------------------------------------------\n");
#endif

  return 0;
}