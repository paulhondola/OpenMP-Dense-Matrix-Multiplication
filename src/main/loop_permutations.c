#include "../common/matrix.h"
#include "../test/test.h"
#include "../utils/utils.h"
#include <omp.h>

int main(int argc, char *argv[]) {

  // Access CSV_DATA variables from utils.h:
  // - compare_serial_loop_permutations_data (CSV_DATA type)
  // - compare_parallel_loop_permutations_data (CSV_DATA type)
  
  // Example usage:
  // FILE *csv = open_csv_file(compare_serial_loop_permutations_data);
  // fprintf(csv, "1000,2.345,1.18,0.67,0.57,0.41,0.38\n");
  // fclose(csv);
  
  // Access fields:
  // printf("Filename: %s\n", compare_serial_loop_permutations_data.filename);
  // printf("Header: %s\n", compare_serial_loop_permutations_data.header);

  Matrix a, b;
  a = matrix_create();
  b = matrix_create();
  matrix_fill_random(a);
  matrix_fill_random(b);

  test_serial_loop_permutations(a, b);
  test_parallel_loop_permutations(a, b, omp_get_num_threads(), 1);

  matrix_destroy(a);
  matrix_destroy(b);

  return 0;
}
