#include "../benchmark/benchmark.h"
#include "../common/matrix.h"
#include "../utils/utils.h"
#include <omp.h>

void benchmark_serial_loop_permutations(Matrix a, Matrix b) {

  FILE *csv_file = open_csv_file(csv_serial_permutations);
  if (csv_file == NULL) {
    perror(csv_serial_permutations.filename);
    exit(1);
  }

  double time_results[LOOP_PERMUTATIONS] = {0};
  double speedup_results[LOOP_PERMUTATIONS] = {0};
  test_serial_loop_permutations(time_results, a, b);
  compute_speedup(time_results, speedup_results, LOOP_PERMUTATIONS);

  fprintf(csv_file, "%d,%f,%f,%f,%f,%f,%f\n", a.size, speedup_results[0],
          speedup_results[1], speedup_results[2], speedup_results[3],
          speedup_results[4], speedup_results[5]);

  fclose(csv_file);
}

void benchmark_parallel_loop_permutations(Matrix a, Matrix b, int thread_count,
                                          int chunk) {
  FILE *csv_file = open_csv_file(csv_parallel_permutations);
  if (csv_file == NULL) {
    perror(csv_parallel_permutations.filename);
    exit(1);
  }

  double time_results[LOOP_PERMUTATIONS] = {0};
  double speedup_results[LOOP_PERMUTATIONS] = {0};
  test_parallel_loop_permutations(time_results, a, b, thread_count, chunk);
  compute_speedup(time_results, speedup_results, LOOP_PERMUTATIONS);

  fprintf(csv_file, "%d,%d,%d,%f,%f,%f,%f,%f,%f\n", a.size, thread_count, chunk,
          speedup_results[0], speedup_results[1], speedup_results[2],
          speedup_results[3], speedup_results[4], speedup_results[5]);

  fclose(csv_file);
}

void benchmark_classic_vs_improved(Matrix a, Matrix b, int chunk) {
  FILE *csv_file = open_csv_file(csv_classic_vs_improved);
  if (csv_file == NULL) {
    perror(csv_classic_vs_improved.filename);
    exit(1);
  }

  double time_results[CLASSIC_VS_IMPROVED_TESTS] = {0};
  double speedup_results[CLASSIC_VS_IMPROVED_TESTS] = {0};
  test_classic_vs_improved(time_results, a, b, chunk);
  compute_speedup(time_results, speedup_results, CLASSIC_VS_IMPROVED_TESTS);

  fprintf(csv_file, "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f\n", a.size, chunk,
          speedup_results[0], speedup_results[1], speedup_results[2],
          speedup_results[3], speedup_results[4], speedup_results[5],
          speedup_results[6], speedup_results[7]);

  fclose(csv_file);
}

int main(int argc, char *argv[]) {

  srand(SEED);

  int matrix_size, thread_count, chunk_size;
  get_args(argc, argv, &matrix_size, &thread_count, &chunk_size);

  Matrix a, b;
  matrix_create(&a, matrix_size);
  matrix_create(&b, matrix_size);
  matrix_fill_random(a);
  matrix_fill_random(b);
  benchmark_serial_loop_permutations(a, b);
  benchmark_parallel_loop_permutations(a, b, thread_count, chunk_size);
  benchmark_classic_vs_improved(a, b, chunk_size);
  matrix_destroy(a);
  matrix_destroy(b);
  return 0;
}
