#include "../benchmark/benchmark.h"
#include "../utils/utils.h"

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

  matrix_destroy(a);
  matrix_destroy(b);
  return 0;
}
