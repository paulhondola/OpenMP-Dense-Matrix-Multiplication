#include "mm_serial.h"
#include "../../common/matrix.h"
#include "../../main/parameters.h"
#include <omp.h>
#include <stdio.h>

serial_loop_benchmark serial_loop_benchmark_functions[] = {
    serial_multiply_ijk, serial_multiply_ikj, serial_multiply_jik,
    serial_multiply_jki, serial_multiply_kij, serial_multiply_kji};

double serial_multiply_ijk(Matrix a, Matrix b, Matrix c) {
  matrix_fill_zero(c);

  double start = omp_get_wtime();

  for (int i = 0; i < a.size; i++)
    for (int j = 0; j < a.size; j++)
      for (int k = 0; k < a.size; k++) {
        c.data[i][j] += a.data[i][k] * b.data[k][j];
      }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Serial - ijk - completed - time: %f\n", result);
#endif

  return result;
}

double serial_multiply_ikj(Matrix a, Matrix b, Matrix c) {
  double temp = 0.0;
  matrix_fill_zero(c);

  double start = omp_get_wtime();

  for (int i = 0; i < a.size; i++) {
    for (int k = 0; k < a.size; k++) {
      temp = a.data[i][k];
      for (int j = 0; j < a.size; j++) {
        c.data[i][j] += temp * b.data[k][j];
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Serial - ikj - completed - time: %f\n", result);
#endif

  return result;
}

double serial_multiply_jik(Matrix a, Matrix b, Matrix c) {
  matrix_fill_zero(c);

  double start = omp_get_wtime();

  for (int j = 0; j < a.size; j++) {
    for (int i = 0; i < a.size; i++) {
      for (int k = 0; k < a.size; k++) {
        c.data[i][j] += a.data[i][k] * b.data[k][j];
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Serial - jik - completed - time: %f\n", result);
#endif

  return result;
}

double serial_multiply_jki(Matrix a, Matrix b, Matrix c) {
  double temp = 0.0;
  matrix_fill_zero(c);

  double start = omp_get_wtime();

  for (int j = 0; j < a.size; j++) {
    for (int k = 0; k < a.size; k++) {
      temp = b.data[k][j];
      for (int i = 0; i < a.size; i++) {
        c.data[i][j] += a.data[i][k] * temp;
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Serial - jki - completed - time: %f\n", result);
#endif

  return result;
}

double serial_multiply_kij(Matrix a, Matrix b, Matrix c) {
  double temp = 0.0;
  matrix_fill_zero(c);

  double start = omp_get_wtime();

  for (int k = 0; k < a.size; k++) {
    for (int i = 0; i < a.size; i++) {
      temp = a.data[i][k];
      for (int j = 0; j < a.size; j++) {
        c.data[i][j] += temp * b.data[k][j];
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Serial - kij - completed - time: %f\n", result);
#endif

  return result;
}

double serial_multiply_kji(Matrix a, Matrix b, Matrix c) {
  double temp = 0.0;
  matrix_fill_zero(c);

  double start = omp_get_wtime();

  for (int k = 0; k < a.size; k++) {
    for (int j = 0; j < a.size; j++) {
      temp = b.data[k][j];
      for (int i = 0; i < a.size; i++) {
        c.data[i][j] += a.data[i][k] * temp;
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Serial - kji - completed - time: %f\n", result);
#endif

  return result;
}