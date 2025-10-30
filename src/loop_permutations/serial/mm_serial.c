#include "../../common/matrix.h"

void serial_multiply_ijk(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  matrix_fill_zero(c);

  for (int i = 0; i < a->rows; i++)
    for (int j = 0; j < b->cols; j++)
      for (int k = 0; k < a->cols; k++)
        c->data[i][j] += a->data[i][k] * b->data[k][j];
}

void serial_multiply_ikj(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  matrix_fill_zero(c);

  for (int i = 0; i < a->rows; i++)
    for (int k = 0; k < a->cols; k++)
      for (int j = 0; j < b->cols; j++)
        c->data[i][j] += a->data[i][k] * b->data[k][j];
}

void serial_multiply_jik(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  matrix_fill_zero(c);

  for (int j = 0; j < b->cols; j++)
    for (int i = 0; i < a->rows; i++)
      for (int k = 0; k < a->cols; k++)
        c->data[i][j] += a->data[i][k] * b->data[k][j];
}

void serial_multiply_jki(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  matrix_fill_zero(c);

  for (int j = 0; j < b->cols; j++)
    for (int k = 0; k < a->cols; k++)
      for (int i = 0; i < a->rows; i++)
        c->data[i][j] += a->data[i][k] * b->data[k][j];
}

void serial_multiply_kij(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  matrix_fill_zero(c);

  for (int k = 0; k < a->cols; k++)
    for (int i = 0; i < a->rows; i++)
      for (int j = 0; j < b->cols; j++)
        c->data[i][j] += a->data[i][k] * b->data[k][j];
}

void serial_multiply_kji(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  matrix_fill_zero(c);

  for (int k = 0; k < a->cols; k++)
    for (int j = 0; j < b->cols; j++)
      for (int i = 0; i < a->rows; i++)
        c->data[i][j] += a->data[i][k] * b->data[k][j];
}