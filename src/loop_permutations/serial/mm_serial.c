#include "../../common/matrix.h"
#include <stdio.h>

void serial_multiply_ijk(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  int N = a->rows;

  matrix_fill_zero(c);

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      for (int k = 0; k < N; k++) {
        c->data[i][j] += a->data[i][k] * b->data[k][j];
      }

#ifdef DEBUG
  printf("Serial multiplication ijk completed\n");
#endif
}

void serial_multiply_ikj(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  int N = a->rows;
  double temp = 0.0;
  matrix_fill_zero(c);

  for (int i = 0; i < N; i++) {
    for (int k = 0; k < N; k++) {
      temp = a->data[i][k];
      for (int j = 0; j < N; j++) {
        c->data[i][j] += temp * b->data[k][j];
      }
    }
  }

#ifdef DEBUG
  printf("Serial multiplication ikj completed\n");
#endif
}

void serial_multiply_jik(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  int N = a->rows;
  matrix_fill_zero(c);

  for (int j = 0; j < N; j++) {
    for (int i = 0; i < N; i++) {
      for (int k = 0; k < N; k++) {
        c->data[i][j] += a->data[i][k] * b->data[k][j];
      }
    }
  }

#ifdef DEBUG
  printf("Serial multiplication jik completed\n");
#endif
}

void serial_multiply_jki(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  int N = a->rows;
  double temp = 0.0;
  matrix_fill_zero(c);

  for (int j = 0; j < N; j++) {
    for (int k = 0; k < N; k++) {
      temp = b->data[k][j];
      for (int i = 0; i < N; i++) {
        c->data[i][j] += a->data[i][k] * temp;
      }
    }
  }

#ifdef DEBUG
  printf("Serial multiplication jki completed\n");
#endif
}

void serial_multiply_kij(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  int N = a->rows;
  double temp = 0.0;
  matrix_fill_zero(c);

  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      temp = a->data[i][k];
      for (int j = 0; j < N; j++) {
        c->data[i][j] += temp * b->data[k][j];
      }
    }
  }

#ifdef DEBUG
  printf("Serial multiplication kij completed\n");
#endif
}

void serial_multiply_kji(Matrix *a, Matrix *b, Matrix *c) {
  if (a->cols != b->rows || a->rows != c->rows || b->cols != c->cols) {
    return;
  }

  int N = a->rows;
  double temp = 0.0;
  matrix_fill_zero(c);

  for (int k = 0; k < N; k++) {
    for (int j = 0; j < N; j++) {
      temp = b->data[k][j];
      for (int i = 0; i < N; i++) {
        c->data[i][j] += a->data[i][k] * temp;
      }
    }
  }

#ifdef DEBUG
  printf("Serial multiplication kji completed\n");
#endif
}