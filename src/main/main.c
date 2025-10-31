#include "../common/matrix.h"
#include "../utils/utils.h"

int main(int argc, char *argv[]) {
  srand(SEED);

  FILE *csv_file = open_csv_file(csv_serial_permutations);
  fclose(csv_file);

  csv_file = open_csv_file(csv_parallel_permutations);
  fclose(csv_file);

  csv_file = open_csv_file(csv_classic_vs_improved);
  fclose(csv_file);

  return 0;
}
