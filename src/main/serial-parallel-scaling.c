#include "../benchmark/benchmark.h"
#include "../utils/utils.h"
#include "parameters.h"

void benchmark_serial_parallel_scaling(
    Matrix a, Matrix b, int chunk, CSV_DATA csv_data,
    int (*test_function)(double time_results[], Matrix a, Matrix b,
                         int chunk)) {
  FILE *csv_file = open_csv_file(csv_data);
  if (csv_file == NULL) {
    perror(csv_data.filename);
    exit(1);
  }

  double time_results[CLASSIC_VS_IMPROVED_TESTS] = {0};
  double speedup_results[CLASSIC_VS_IMPROVED_TESTS] = {0};
  test_function(time_results, a, b, chunk);
  compute_speedup(time_results, speedup_results, CLASSIC_VS_IMPROVED_TESTS);

  fprintf(csv_file, "%d,%d,%f,%f,%f,%f\n", a.size, chunk, speedup_results[0],
          speedup_results[1], speedup_results[2], speedup_results[3]);

  fclose(csv_file);
}

void run_benchmark(int matrix_size, int chunk_size) {
  Matrix a, b;
  matrix_create(&a, matrix_size);
  matrix_create(&b, matrix_size);
  matrix_fill_random(a);
  matrix_fill_random(b);

  benchmark_serial_parallel_scaling(a, b, chunk_size,
                                    csv_serial_parallel_scaling_classic,
                                    test_serial_parallel_scaling_classic);
  benchmark_serial_parallel_scaling(a, b, chunk_size,
                                    csv_serial_parallel_scaling_improved,
                                    test_serial_parallel_scaling_improved);

  matrix_destroy(a);
  matrix_destroy(b);
}

int main(int argc, char *argv[]) {
  // Set output folder if provided as command line argument
  const char *folder_name = (argc > 1) ? argv[1] : NULL;
  set_output_folder(folder_name);
  
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
