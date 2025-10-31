#include "../../common/matrix.h"
#include <stdio.h>

void serial_multiply_ijk(Matrix a, Matrix b, Matrix c) {
  matrix_fill_zero(c);

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      for (int k = 0; k < N; k++) {
        c[i][j] += a[i][k] * b[k][j];
      }

#ifdef DEBUG
  printf("Serial - ijk - completed\n");
#endif
}

void serial_multiply_ikj(Matrix a, Matrix b, Matrix c) {
  double temp = 0.0;
  matrix_fill_zero(c);

  for (int i = 0; i < N; i++) {
    for (int k = 0; k < N; k++) {
      temp = a[i][k];
      for (int j = 0; j < N; j++) {
        c[i][j] += temp * b[k][j];
      }
    }
  }

#ifdef DEBUG
  printf("Serial - ikj - completed\n");
#endif
}

void serial_multiply_jik(Matrix a, Matrix b, Matrix c) {
  matrix_fill_zero(c);

  for (int j = 0; j < N; j++) {
    for (int i = 0; i < N; i++) {
      for (int k = 0; k < N; k++) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }

#ifdef DEBUG
  printf("Serial - jik - completed\n");
#endif
}

void serial_multiply_jki(Matrix a, Matrix b, Matrix c) {
  double temp = 0.0;
  matrix_fill_zero(c);

  for (int j = 0; j < N; j++) {
    for (int k = 0; k < N; k++) {
      temp = b[k][j];
      for (int i = 0; i < N; i++) {
        c[i][j] += a[i][k] * temp;
      }
    }
  }

#ifdef DEBUG
  printf("Serial - jki - completed\n");
#endif
}

void serial_multiply_kij(Matrix a, Matrix b, Matrix c) {
  double temp = 0.0;
  matrix_fill_zero(c);

  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      temp = a[i][k];
      for (int j = 0; j < N; j++) {
        c[i][j] += temp * b[k][j];
      }
    }
  }

#ifdef DEBUG
  printf("Serial - kij - completed\n");
#endif
}

void serial_multiply_kji(Matrix a, Matrix b, Matrix c) {
  double temp = 0.0;
  matrix_fill_zero(c);

  for (int k = 0; k < N; k++) {
    for (int j = 0; j < N; j++) {
      temp = b[k][j];
      for (int i = 0; i < N; i++) {
        c[i][j] += a[i][k] * temp;
      }
    }
  }

#ifdef DEBUG
  printf("Serial - kji - completed\n");
#endif
}