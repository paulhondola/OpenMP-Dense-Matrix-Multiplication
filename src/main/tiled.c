#include "../benchmark/benchmark.h"
#include "../common/matrix.h"
#include "../utils/utils.h"

void benchmark_tiled(Matrix a, Matrix b, int thread_count, int block_size) {
  FILE *csv_file = open_csv_file(csv_tiled);
  if (csv_file == NULL) {
    perror(csv_tiled.filename);
    exit(1);
  }

  double time_results[TILED_TESTS] = {0};
  double speedup_results[TILED_TESTS] = {0};
  test_tiled(time_results, a, b, thread_count, block_size);
  compute_speedup(time_results, speedup_results, TILED_TESTS);

  fprintf(csv_file, "%d,%d,%d,%f,%f,%f,%f\n", a.size, thread_count, block_size,
          speedup_results[0], speedup_results[1], speedup_results[2],
          speedup_results[3]);

  fclose(csv_file);
}

int main(int argc, char *argv[]) {

  srand(SEED);

  int matrix_size, block_size, thread_count;
  get_args(argc, argv, &matrix_size, &thread_count, &block_size);

  Matrix a, b;
  matrix_create(&a, matrix_size);
  matrix_create(&b, matrix_size);
  matrix_fill_random(a);
  matrix_fill_random(b);
  benchmark_tiled(a, b, thread_count, block_size);
  matrix_destroy(a);
  matrix_destroy(b);
  return 0;
}
