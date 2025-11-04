#include "../benchmark/benchmark.h"
#include "../utils/utils.h"
#include "parameters.h"

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

void run_benchmark(int matrix_size, int chunk_size) {
  Matrix a, b;
  matrix_create(&a, matrix_size);
  matrix_create(&b, matrix_size);
  matrix_fill_random(a);
  matrix_fill_random(b);

  benchmark_classic_vs_improved(a, b, chunk_size);

  matrix_destroy(a);
  matrix_destroy(b);
}

int main(void) {
  srand(SEED);

  int matrix_sizes[] = MATRIX_SIZES;
  int chunk_sizes[] = CHUNK_SIZES;

  int num_matrix_sizes = sizeof(matrix_sizes) / sizeof(matrix_sizes[0]);
  int num_chunk_sizes = sizeof(chunk_sizes) / sizeof(chunk_sizes[0]);

  for (int i = 0; i < num_matrix_sizes; i++) {
    for (int j = 0; j < num_chunk_sizes; j++) {
      int matrix_size = matrix_sizes[i];
      int chunk_size = chunk_sizes[j];
      run_benchmark(matrix_size, chunk_size);
    }
  }

  return 0;
}
