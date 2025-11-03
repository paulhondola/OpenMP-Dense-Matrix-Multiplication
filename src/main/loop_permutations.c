#include "../benchmark/benchmark.h"
#include "../common/matrix.h"
#include "../utils/utils.h"
#include <omp.h>

void benchmark_serial_loop_permutations(Matrix a, Matrix b,
                                        double time_results[PERMUTATIONS]) {

  FILE *csv_file = open_csv_file(csv_serial_permutations);
  if (csv_file == NULL) {
    perror(csv_serial_permutations.filename);
    exit(1);
  }

  test_serial_loop_permutations(a, b, time_results);
  fprintf(csv_file, "%d,%f,%f,%f,%f,%f,%f\n", a.size, time_results[0],
          time_results[1], time_results[2], time_results[3], time_results[4],
          time_results[5]);

  fclose(csv_file);
}

void benchmark_parallel_loop_permutations(Matrix a, Matrix b, int thread_count,
                                          int chunk,
                                          double time_results[PERMUTATIONS]) {
  FILE *csv_file = open_csv_file(csv_parallel_permutations);
  if (csv_file == NULL) {
    perror(csv_parallel_permutations.filename);
    exit(1);
  }

  test_parallel_loop_permutations(a, b, thread_count, chunk, time_results);
  fprintf(csv_file, "%d,%d,%d,%f,%f,%f,%f,%f,%f\n", a.size, thread_count, chunk,
          time_results[0], time_results[1], time_results[2], time_results[3],
          time_results[4], time_results[5]);

  fclose(csv_file);
}

void benchmark_classic_vs_improved(Matrix a, Matrix b, int chunk,
                                   double time_results[]) {
  FILE *csv_file = open_csv_file(csv_classic_vs_improved);
  if (csv_file == NULL) {
    perror(csv_classic_vs_improved.filename);
    exit(1);
  }

  test_classic_vs_improved(a, b, chunk, time_results);
  fprintf(csv_file, "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f\n", a.size, chunk,
          time_results[0], time_results[1], time_results[2], time_results[3],
          time_results[4], time_results[5], time_results[6], time_results[7]);

  fclose(csv_file);
}

int main(int argc, char *argv[]) {

  srand(SEED);

  int matrix_size, thread_count, chunk_size;

  get_args(argc, argv, &matrix_size, &thread_count, &chunk_size);

  double time_results[PERMUTATIONS] = {0};
  Matrix a, b;
  matrix_create(&a, matrix_size);
  matrix_create(&b, matrix_size);
  matrix_fill_random(a);
  matrix_fill_random(b);
  benchmark_serial_loop_permutations(a, b, time_results);
  benchmark_parallel_loop_permutations(a, b, thread_count, chunk_size,
                                       time_results);
  benchmark_classic_vs_improved(a, b, chunk_size, time_results);
  matrix_destroy(a);
  matrix_destroy(b);
  return 0;
}
