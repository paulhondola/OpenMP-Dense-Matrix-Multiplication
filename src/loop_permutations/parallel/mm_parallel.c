#include "mm_parallel.h"
#include <omp.h>

parallel_loop_benchmark parallel_loop_benchmark_functions[] = {
    parallel_multiply_ijk, parallel_multiply_ikj, parallel_multiply_jik,
    parallel_multiply_jki, parallel_multiply_kji, parallel_multiply_kji};

double parallel_multiply_ijk(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k, n = a.size;
  double temp;
  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk, n)
  {
#pragma omp for schedule(static, chunk)
    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        temp = 0;
        for (k = 0; k < n; k++) {
          temp += a.data[i][k] * b.data[k][j];
        }
        c.data[i][j] = temp;
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Parallel - ijk - completed - time: %f\n", result);
#endif

  return result;
}

double parallel_multiply_ikj(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k, n = a.size;
  double temp;
  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk, n)
  {

#pragma omp for schedule(static, chunk)
    for (i = 0; i < n; i++) {
      for (k = 0; k < n; k++) {
        temp = a.data[i][k];
        for (j = 0; j < n; j++) {
          c.data[i][j] += temp * b.data[k][j];
        }
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Parallel - ikj - completed - time: %f\n", result);
#endif

  return result;
}

double parallel_multiply_jik(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k, n = a.size;
  double temp;
  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk, n)
  {
#pragma omp for schedule(static, chunk)
    for (j = 0; j < n; j++) {
      for (i = 0; i < n; i++) {
        temp = 0;
        for (k = 0; k < n; k++) {
          temp += a.data[i][k] * b.data[k][j];
        }
        c.data[i][j] = temp;
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Parallel - jik - completed - time: %f\n", result);
#endif

  return result;
}

double parallel_multiply_jki(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k, n = a.size;
  double temp;

  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk, n)
  {
#pragma omp for schedule(static, chunk)
    for (j = 0; j < n; j++) {
      for (k = 0; k < n; k++) {
        temp = b.data[k][j];
        for (i = 0; i < n; i++) {
          c.data[i][j] += a.data[i][k] * temp;
        }
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Parallel - jki - completed - time: %f\n", result);
#endif

  return result;
}

double parallel_multiply_kij(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k, n = a.size;
  double temp;

  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk, n)
  {
    for (k = 0; k < n; k++) {
#pragma omp for schedule(static, chunk)
      for (i = 0; i < n; i++) {
        temp = a.data[i][k];
        for (j = 0; j < n; j++) {
          c.data[i][j] += temp * b.data[k][j];
        }
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Parallel - kij - completed - time: %f\n", result);
#endif

  return result;
}

double parallel_multiply_kji(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int n = a.size;
  int i, j, k;
  double temp;

  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk, n)
  {
    for (k = 0; k < n; k++) {
#pragma omp for schedule(static, chunk)
      for (j = 0; j < n; j++) {
        temp = b.data[k][j];
        for (i = 0; i < n; i++) {
          c.data[i][j] += a.data[i][k] * temp;
        }
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Parallel - kji - completed - time: %f\n", result);
#endif

  return result;
}
