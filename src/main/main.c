#include "../common/matrix.h"
#include "../utils/utils.h"

int main(int argc, char *argv[]) {
  clear_csv_file(csv_serial_permutations);
  clear_csv_file(csv_parallel_permutations);
  clear_csv_file(csv_classic_vs_improved);
  clear_csv_file(csv_tiled);
  return 0;
}
