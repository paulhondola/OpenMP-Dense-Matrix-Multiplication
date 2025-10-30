#include "../common/matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 5

int main(int argc, char *argv[]) {
  srand(SEED);

  Matrix matrix;
  matrix_create(&matrix, N, N);
  matrix_fill_random(&matrix);
  matrix_print(&matrix);
  matrix_destroy(&matrix);
  return 0;
}