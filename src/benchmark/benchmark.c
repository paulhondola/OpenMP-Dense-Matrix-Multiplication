#include "benchmark.h"
#include "../common/matrix.h"
#include "../loop_permutations/parallel/mm_parallel.h"
#include "../loop_permutations/serial/mm_serial.h"

int run_serial_loop_permutation(double time_results[], Matrix a, Matrix b,
                                Matrix reference, int permutation) {

  Matrix c;
  matrix_create(&c, a.size);
  time_results[permutation] =
      serial_loop_benchmark_functions[permutation](a, b, c);
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

int test_serial_loop_permutations(double time_results[], Matrix a, Matrix b) {

#ifdef DEBUG
  printf("Serial - Testing loop "
         "permutations\n------------------------------------------\n");
#endif

  Matrix reference;
  matrix_create(&reference, a.size);
  time_results[0] = serial_loop_benchmark_functions[0](a, b, reference);
#ifdef DEBUG
  printf("Time: %f seconds\n", time_results[0]);
  printf("------------------------------------------\n");
#endif

  int correct_count = 0;
  for (int i = 1; i < LOOP_PERMUTATIONS; i++) {
    if (run_serial_loop_permutation(time_results, a, b, reference, i)) {
      correct_count++;
    }
  }

  matrix_destroy(reference);

#ifdef DEBUG
  printf("Serial - Test completed\n");
  printf("Correct count: %d\n", correct_count);
  printf("Total permutations: %d\n", LOOP_PERMUTATIONS - 1);
  printf("------------------------------------------\n");
#endif

  return correct_count == LOOP_PERMUTATIONS - 1;
}

int run_parallel_loop_permutation(double time_results[], Matrix a, Matrix b,
                                  Matrix reference, int thread_count, int chunk,
                                  int permutation) {

  Matrix c;
  matrix_create(&c, a.size);
  time_results[permutation] = parallel_loop_benchmark_functions[permutation](
      a, b, c, thread_count, chunk);
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

int test_parallel_loop_permutations(double time_results[], Matrix a, Matrix b,
                                    int thread_count, int chunk) {

#ifdef DEBUG
  printf("Parallel - Testing loop "
         "permutations\n------------------------------------------\n");
#endif

  // set the IJK permutation as reference
  Matrix reference;
  matrix_create(&reference, a.size);
  time_results[0] = parallel_loop_benchmark_functions[0](a, b, reference,
                                                         thread_count, chunk);

#ifdef DEBUG
  printf("------------------------------------------\n");
#endif

  int correct_count = 0;
  for (int i = 1; i < LOOP_PERMUTATIONS; i++) {
    if (run_parallel_loop_permutation(time_results, a, b, reference,
                                      thread_count, chunk, i)) {
      correct_count++;
    }
  }

  matrix_destroy(reference);

#ifdef DEBUG
  printf("Parallel - Test completed\n");
  printf("Correct count: %d\n", correct_count);
  printf("Total permutations: %d\n", LOOP_PERMUTATIONS - 1);
  printf("------------------------------------------\n");
#endif

  return correct_count == LOOP_PERMUTATIONS - 1;
}

int test_classic_vs_improved(double time_results[], Matrix a, Matrix b,
                             int chunk) {

  Matrix c;
  matrix_create(&c, a.size);
  time_results[0] = serial_multiply_ijk(a, b, c);
  time_results[1] = parallel_multiply_ijk(a, b, c, 2, chunk);
  time_results[2] = parallel_multiply_ijk(a, b, c, 4, chunk);
  time_results[3] = parallel_multiply_ijk(a, b, c, 8, chunk);
  time_results[4] = serial_multiply_ikj(a, b, c);
  time_results[5] = parallel_multiply_ikj(a, b, c, 2, chunk);
  time_results[6] = parallel_multiply_ikj(a, b, c, 4, chunk);
  time_results[7] = parallel_multiply_ikj(a, b, c, 8, chunk);
  matrix_destroy(c);

#ifdef DEBUG
  printf("Classic vs Improved - Test completed\n");
  printf("------------------------------------------\n");
#endif

  return 0;
}

void compute_speedup(double time_results[], double speedup_results[],
                     int size) {
  for (int i = 0; i < size; i++) {
    speedup_results[i] = time_results[0] / time_results[i];
  }
}
