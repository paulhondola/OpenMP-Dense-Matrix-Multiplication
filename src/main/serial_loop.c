#include "../benchmark/benchmark.h"
#include "../utils/utils.h"
#include "parameters.h"

void benchmark_serial_loop_permutations(const Matrix *restrict a, const Matrix *restrict b) {

  FILE *csv_file = open_csv_file(csv_serial_permutations);
  if (csv_file == NULL) {
    perror(csv_serial_permutations.filename);
    exit(1);
  }

  double time_results[LOOP_PERMUTATIONS] = {0};
  double speedup_results[LOOP_PERMUTATIONS] = {0};
  test_serial_loop_permutations(time_results, a, b);
  compute_speedup(time_results, speedup_results, LOOP_PERMUTATIONS);

  fprintf(csv_file, "%d,%f,%f,%f,%f,%f,%f\n", a->size, speedup_results[0],
          speedup_results[1], speedup_results[2], speedup_results[3],
          speedup_results[4], speedup_results[5]);

  fclose(csv_file);
}

void run_benchmark(int matrix_size) {
  Matrix a, b;
  matrix_create(&a, matrix_size);
  matrix_create(&b, matrix_size);
  matrix_fill_random(&a);
  matrix_fill_random(&b);

  benchmark_serial_loop_permutations(&a, &b);

  matrix_destroy(&a);
  matrix_destroy(&b);
}

int main(int argc, char *argv[]) {
  // Set output folder if provided as command line argument
  const char *folder_name = (argc > 1) ? argv[1] : NULL;
  set_output_folder(folder_name);
  
  srand(SEED);

  int matrix_sizes[] = MATRIX_SIZES;

  int num_matrix_sizes = sizeof(matrix_sizes) / sizeof(matrix_sizes[0]);

  for (int i = 0; i < num_matrix_sizes; i++) {
    int matrix_size = matrix_sizes[i];
    run_benchmark(matrix_size);
  }

  return 0;
}
