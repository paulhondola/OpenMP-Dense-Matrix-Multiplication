#include "../benchmark/benchmark.h"
#include "../utils/utils.h"
#include "parameters.h"

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

void run_benchmark(int matrix_size, int thread_count, int block_size) {
  Matrix a, b;
  matrix_create(&a, matrix_size);
  matrix_create(&b, matrix_size);
  matrix_fill_random(a);
  matrix_fill_random(b);

  benchmark_tiled(a, b, thread_count, block_size);

  matrix_destroy(a);
  matrix_destroy(b);
}

int main(int argc, char *argv[]) {
  // Set output folder if provided as command line argument
  const char *folder_name = (argc > 1) ? argv[1] : NULL;
  set_output_folder(folder_name);
  
  srand(SEED);

  int matrix_sizes[] = MATRIX_SIZES;
  int block_sizes[] = BLOCK_SIZES;
  int thread_count = THREAD_COUNT;

  int num_matrix_sizes = sizeof(matrix_sizes) / sizeof(matrix_sizes[0]);
  int num_block_sizes = sizeof(block_sizes) / sizeof(block_sizes[0]);

  for (int i = 0; i < num_matrix_sizes; i++) {
    for (int j = 0; j < num_block_sizes; j++) {
      int matrix_size = matrix_sizes[i];
      int block_size = block_sizes[j];
      run_benchmark(matrix_size, thread_count, block_size);
    }
  }

  return 0;
}
