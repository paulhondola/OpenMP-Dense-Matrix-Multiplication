#include "test.h"
#include "../common/matrix.h"
#include "../loop_permutations/serial/mm_serial.h"
#include <math.h>
#include <stdio.h>

int validate(Matrix *a, Matrix *b) {
  if (a->rows != b->rows || a->cols != b->cols)
    return 0;

  for (int i = 0; i < a->rows; i++)
    for (int j = 0; j < a->cols; j++)
      if (fabs(a->data[i][j] - b->data[i][j]) > EPSILON)
        return 0;

  return 1;
}

int run_loop_permutation(Matrix *a, Matrix *b, Matrix *reference,
                         int permutation) {
  Matrix c;
  matrix_create(&c, a->rows, a->cols);
  switch (permutation) {
  case 1:
    serial_multiply_ikj(a, b, &c);
    break;
  case 2:
    serial_multiply_jik(a, b, &c);
    break;
  case 3:
    serial_multiply_jki(a, b, &c);
    break;
  case 4:
    serial_multiply_kij(a, b, &c);
    break;
  case 5:
    serial_multiply_kji(a, b, &c);
    break;
  default:
    return 0;
  }

  int result = validate(reference, &c);
  matrix_destroy(&c);

#ifdef DEBUG
  if (result) {
    printf("Permutation %d is correct\n", permutation);
  } else {
    printf("Permutation %d is incorrect\n", permutation);
  }
  printf("------------------------------------------\n");
#endif

  return result;
}

#define NUM_PERMUTATIONS 6
int test_loop_permutations(Matrix *a, Matrix *b) {

#ifdef DEBUG
  printf("Testing loop "
         "permutations\n------------------------------------------\n");
#endif

  Matrix c[NUM_PERMUTATIONS];

  // set the IJK permutation as reference
  matrix_create(&c[0], a->rows, a->cols);
  serial_multiply_ijk(a, b, &c[0]);

  int correct_count = 0;

  for (int i = 1; i < NUM_PERMUTATIONS; i++) {
    if (run_loop_permutation(a, b, &c[0], i)) {
      correct_count++;
    }
  }

#ifdef DEBUG
  printf("Test completed\n");
  printf("Correct count: %d\n", correct_count);
  printf("Total permutations: %d\n", NUM_PERMUTATIONS - 1);
  printf("Accuracy: %f\n", (float)correct_count / (NUM_PERMUTATIONS - 1));
  printf("------------------------------------------\n");
#endif

  return correct_count == NUM_PERMUTATIONS - 1;
}
