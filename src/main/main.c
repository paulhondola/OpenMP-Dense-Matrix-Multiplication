#include "../common/matrix.h"

int main(int argc, char *argv[]) {
  srand(SEED);

  Matrix matrix = matrix_create();
  matrix_fill_random(matrix);
  matrix_print(matrix);
  matrix_destroy(matrix);
  return 0;
}