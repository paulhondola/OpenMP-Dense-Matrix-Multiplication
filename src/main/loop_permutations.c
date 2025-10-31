#include "../benchmark/benchmark.h"
#include "../common/matrix.h"
#include "../utils/utils.h"
#include <omp.h>

void benchmark_serial_loop_permutations(Matrix a, Matrix b,
                                        double times[PERMUTATIONS]) {

  FILE *csv_file = open_csv_file(csv_serial_permutations);
  if (csv_file == NULL) {
    perror(csv_serial_permutations.filename);
    exit(1);
  }

  test_serial_loop_permutations(a, b, times);
  fprintf(csv_file, "%d,%f,%f,%f,%f,%f,%f\n", N, times[0], times[1], times[2],
          times[3], times[4], times[5]);

  fclose(csv_file);
}

void benchmark_parallel_loop_permutations(Matrix a, Matrix b, int thread_count,
                                          int chunk,
                                          double times[PERMUTATIONS]) {
  FILE *csv_file = open_csv_file(csv_parallel_permutations);
  if (csv_file == NULL) {
    perror(csv_parallel_permutations.filename);
    exit(1);
  }

  test_parallel_loop_permutations(a, b, thread_count, chunk, times);
  fprintf(csv_file, "%d,%d,%d,%f,%f,%f,%f,%f,%f\n", N, thread_count, chunk,
          times[0], times[1], times[2], times[3], times[4], times[5]);

  fclose(csv_file);
}

void benchmark_classic_vs_improved(Matrix a, Matrix b, double times[]) {
  FILE *csv_file = open_csv_file(csv_classic_vs_improved);
  if (csv_file == NULL) {
    perror(csv_classic_vs_improved.filename);
    exit(1);
  }

  test_classic_vs_improved(a, b, times);
  fprintf(csv_file, "%d,%f,%f,%f,%f,%f,%f,%f,%f\n", N, times[0], times[1],
          times[2], times[3], times[4], times[5], times[6], times[7]);

  fclose(csv_file);
}

int main(int argc, char *argv[]) {

  srand(SEED);

  double times[PERMUTATIONS] = {0};
  Matrix a = matrix_create();
  Matrix b = matrix_create();
  matrix_fill_random(a);
  matrix_fill_random(b);
  // benchmark_serial_loop_permutations(a, b, times);
  //  benchmark_parallel_loop_permutations(a, b, 10, 100, times);
  benchmark_classic_vs_improved(a, b, times);
  matrix_destroy(a);
  matrix_destroy(b);
  return 0;
}
