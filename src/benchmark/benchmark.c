#include "benchmark.h"
#include "../loop_permutations/parallel/mm_parallel.h"
#include "../loop_permutations/serial/mm_serial.h"
#include "../main/parameters.h"
#include "../tiled/parallel/mm_tiled_parallel.h"
#include "../tiled/serial/mm_tiled_serial.h"

int run_serial_loop_permutation(double time_results[], Matrix a, Matrix b,
                                Matrix reference, int permutation) {

  Matrix c;
  matrix_create(&c, a.size);
  time_results[permutation] =
      serial_loop_benchmark_functions[permutation](a, b, c);
  int result = 1;
#ifdef DEBUG
  result = validate(reference, c);
  if (result) {
    printf(GREEN "Permutation %d is correct" RESET "\n", permutation);
  } else {
    printf(RED "Permutation %d is incorrect" RESET "\n", permutation);
  }
  printf("---------------------------------------------------------------------"
         "---------------------------------------------------\n");
#endif

  matrix_destroy(c);
  return result;
}

int test_serial_loop_permutations(double time_results[], Matrix a, Matrix b) {

#ifdef DEBUG
  printf("Serial - Testing loop permutations - matrix size: %d\n", a.size);
  printf("---------------------------------------------------------------------"
         "---------------------------------------------------\n");
#endif

  Matrix reference;
  matrix_create(&reference, a.size);
  time_results[0] = serial_loop_benchmark_functions[0](a, b, reference);

  int correct_count = 0;
  for (int i = 1; i < LOOP_PERMUTATIONS; i++) {
    if (run_serial_loop_permutation(time_results, a, b, reference, i)) {
      correct_count++;
    }
  }

  matrix_destroy(reference);

#ifdef DEBUG
  printf("Serial - Test completed - matrix size: %d\n", a.size);
  printf("Correct count: %d\n", correct_count);
  printf("Total permutations: %d\n", LOOP_PERMUTATIONS - 1);
  printf("---------------------------------------------------------------------"
         "---------------------------------------------------\n");
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
  int result = 1;
#ifdef DEBUG
  result = validate(reference, c);
  if (result) {
    printf(GREEN "Permutation %d is correct" RESET "\n", permutation);
  } else {
    printf(RED "Permutation %d is incorrect" RESET "\n", permutation);
  }
  printf("---------------------------------------------------------------------"
         "---------------------------------------------------\n");
#endif
  matrix_destroy(c);

  return result;
}

int test_parallel_loop_permutations(double time_results[], Matrix a, Matrix b,
                                    int thread_count, int chunk_size) {

#ifdef DEBUG
  printf("Parallel - Testing loop permutations - matrix size: %d, threads: %d, "
         "chunk: %d\n",
         a.size, thread_count, chunk_size);
  printf("---------------------------------------------------------------------"
         "---------------------------------------------------\n");
#endif

  // set the IJK permutation as reference
  Matrix reference;
  matrix_create(&reference, a.size);
  time_results[0] = parallel_loop_benchmark_functions[0](
      a, b, reference, thread_count, chunk_size);

  int correct_count = 0;
  for (int i = 1; i < LOOP_PERMUTATIONS; i++) {
    if (run_parallel_loop_permutation(time_results, a, b, reference,
                                      thread_count, chunk_size, i)) {
      correct_count++;
    }
  }

  matrix_destroy(reference);

#ifdef DEBUG
  printf(
      "Parallel - Test completed - matrix size: %d, threads: %d, chunk: %d\n",
      a.size, thread_count, chunk_size);
  printf("Correct count: %d\n", correct_count);
  printf("Total permutations: %d\n", LOOP_PERMUTATIONS - 1);
  printf("---------------------------------------------------------------------"
         "---------------------------------------------------\n");
#endif

  return correct_count == LOOP_PERMUTATIONS - 1;
}

int test_serial_parallel_scaling_classic(double time_results[], Matrix a,
                                         Matrix b, int chunk) {

  Matrix c;
  matrix_create(&c, a.size);
  time_results[0] = serial_multiply_ijk(a, b, c);
  time_results[1] = parallel_multiply_ijk(a, b, c, 2, chunk);
  time_results[2] = parallel_multiply_ijk(a, b, c, 4, chunk);
  time_results[3] = parallel_multiply_ijk(a, b, c, 8, chunk);
  matrix_destroy(c);

#ifdef DEBUG
  printf("Serial - parallel scaling - improved - matrix size: %d, chunk: %d\n",
         a.size, chunk);
  printf("---------------------------------------------------------------------"
         "---------------------------------------------------\n");
#endif

  return 0;
}

int test_serial_parallel_scaling_improved(double time_results[], Matrix a,
                                          Matrix b, int chunk) {
  Matrix c;
  matrix_create(&c, a.size);
  time_results[0] = serial_multiply_ikj(a, b, c);
  time_results[1] = parallel_multiply_ikj(a, b, c, 2, chunk);
  time_results[2] = parallel_multiply_ikj(a, b, c, 4, chunk);
  time_results[3] = parallel_multiply_ikj(a, b, c, 8, chunk);
  matrix_destroy(c);

#ifdef DEBUG
  printf("Serial - parallel scaling - improved - matrix size: %d, chunk: %d\n",
         a.size, chunk);
  printf("---------------------------------------------------------------------"
         "---------------------------------------------------\n");
#endif

  return 0;
}

int test_tiled(double time_results[], Matrix a, Matrix b, int thread_count,
               int block_size) {
  Matrix reference, c;
  matrix_create(&reference, a.size);
  matrix_create(&c, a.size);

  time_results[0] = serial_multiply_ikj(a, b, reference);
  time_results[1] = parallel_multiply_ikj(a, b, c, thread_count, block_size);

  int result = 1;
#ifdef DEBUG_MATRIX
  matrix_print(c);
#endif

#ifdef DEBUG
  printf("---------------------------------------------------------------------"
         "---------------------------------------------------\n");
#endif

  time_results[2] = serial_multiply_tiled(a, b, c, block_size);

#ifdef DEBUG_MATRIX
  matrix_print(c);
#endif

#ifdef DEBUG
  result = validate(reference, c);
  if (result) {
    printf(GREEN "Serial - tiled - matrix size: %d, block size: %d - "
                 "completed - "
                 "time: %f" RESET "\n",
           a.size, block_size, time_results[2]);
  } else {
    printf(RED "INCORRECT - Serial - tiled - matrix size: %d, block "
               "size: %d" RESET "\n",
           a.size, block_size);
  }
#endif

  time_results[3] = parallel_multiply_tiled(a, b, c, thread_count, block_size);

#ifdef DEBUG_MATRIX
  matrix_print(c);
#endif

#ifdef DEBUG
  result = validate(reference, c);
  if (result) {
    printf(GREEN "Parallel - tiled - matrix size: %d, threads: %d, block "
                 "size: %d - completed - time: %f" RESET "\n",
           a.size, thread_count, block_size, time_results[3]);
  } else {
    printf(RED "INCORRECT - Parallel - tiled - matrix size: %d, threads: "
               "%d, block size: %d" RESET "\n",
           a.size, thread_count, block_size);
  }
#endif

  time_results[4] =
      parallel_multiply_tiled_tasks(a, b, c, thread_count, block_size);

#ifdef DEBUG_MATRIX
  matrix_print(c);
#endif

#ifdef DEBUG
  result = validate(reference, c);
  if (result) {
    printf(GREEN "Parallel - tiled tasks - matrix size: %d, threads: %d, block "
                 "size: %d - completed - time: %f" RESET "\n",
           a.size, thread_count, block_size, time_results[4]);
  } else {
    printf(RED "INCORRECT - Parallel - tiled tasks - matrix size: %d, threads: "
               "%d, block size: %d" RESET "\n",
           a.size, thread_count, block_size);
  }
  printf("---------------------------------------------------------------------"
         "---------------------------------------------------\n");
#endif

  matrix_destroy(reference);
  matrix_destroy(c);

  return result;
}

void compute_speedup(double time_results[], double speedup_results[],
                     int size) {
  for (int i = 0; i < size; i++) {
    speedup_results[i] = time_results[0] / time_results[i];
  }
}
