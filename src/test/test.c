#include "test.h"
#include <math.h>

bool validate(Matrix *matrix1, Matrix *matrix2) {
  if (matrix1->rows != matrix2->rows || matrix1->cols != matrix2->cols)
    return false;

  for (int i = 0; i < matrix1->rows; i++)
    for (int j = 0; j < matrix1->cols; j++)
      if (fabs(matrix1->data[i][j] - matrix2->data[i][j]) > EPSILON)
        return false;

  return true;
}
