#include "../common/matrix.h"
#include "../utils/utils.h"

int main(int argc, char *argv[]) {
  srand(SEED);

  FILE *csv_file = open_csv_file(compare_serial_loop_permutations_data);
  fclose(csv_file);

  csv_file = open_csv_file(compare_parallel_loop_permutations_data);
  fclose(csv_file);

  csv_file = open_csv_file(compare_classic_vs_improved_data);
  fclose(csv_file);

  return 0;
}
