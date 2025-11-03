#include "mm_parallel.h"
#include <omp.h>

parallel_loop_benchmark parallel_loop_benchmark_functions[] = {
    parallel_multiply_ijk, parallel_multiply_ikj, parallel_multiply_jik,
    parallel_multiply_jki, parallel_multiply_kji, parallel_multiply_kji};

double parallel_multiply_ijk(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k;
  double temp;
  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk)
  {
#pragma omp for schedule(static, chunk)
    for (i = 0; i < a.size; i++) {
      for (j = 0; j < a.size; j++) {
        temp = 0;
        for (k = 0; k < a.size; k++) {
          temp += a.data[i][k] * b.data[k][j];
        }
        c.data[i][j] = temp;
      }
    }
  }

  double end = omp_get_wtime();

#ifdef DEBUG
  printf("Parallel - ijk - completed\n");
#endif

  return end - start;
}

double parallel_multiply_ikj(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k;
  double temp;
  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk)
  {

#pragma omp for schedule(static, chunk)
    for (i = 0; i < a.size; i++) {
      for (k = 0; k < a.size; k++) {
        temp = a.data[i][k];
        for (j = 0; j < a.size; j++) {
          c.data[i][j] += temp * b.data[k][j];
        }
      }
    }
  }

  double end = omp_get_wtime();

#ifdef DEBUG
  printf("Parallel - ikj - completed\n");
#endif

  return end - start;
}

double parallel_multiply_jik(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k;
  double temp;
  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk)
  {
#pragma omp for schedule(static, chunk)
    for (j = 0; j < a.size; j++) {
      for (i = 0; i < a.size; i++) {
        temp = 0;
        for (k = 0; k < a.size; k++) {
          temp += a.data[i][k] * b.data[k][j];
        }
        c.data[i][j] = temp;
      }
    }
  }

  double end = omp_get_wtime();

#ifdef DEBUG
  printf("Parallel - jik - completed\n");
#endif

  return end - start;
}

double parallel_multiply_jki(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k;
  double temp;

  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk)
  {
#pragma omp for schedule(static, chunk)
    for (j = 0; j < a.size; j++) {
      for (k = 0; k < a.size; k++) {
        temp = b.data[k][j];
        for (i = 0; i < a.size; i++) {
          c.data[i][j] += a.data[i][k] * temp;
        }
      }
    }
  }

  double end = omp_get_wtime();

#ifdef DEBUG
  printf("Parallel - jki - completed\n");
#endif

  return end - start;
}

double parallel_multiply_kij(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k;
  double temp;

  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk)
  {
    for (k = 0; k < a.size; k++) {
#pragma omp for schedule(static, chunk)
      for (i = 0; i < a.size; i++) {
        temp = a.data[i][k];
        for (j = 0; j < a.size; j++) {
          c.data[i][j] += temp * b.data[k][j];
        }
      }
    }
  }

  double end = omp_get_wtime();

#ifdef DEBUG
  printf("Parallel - kij - completed\n");
#endif

  return end - start;
}

double parallel_multiply_kji(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k;
  double temp;

  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k, temp), shared(a, b, c, chunk)
  {
    for (k = 0; k < a.size; k++) {
#pragma omp for schedule(static, chunk)
      for (j = 0; j < a.size; j++) {
        temp = b.data[k][j];
        for (i = 0; i < a.size; i++) {
          c.data[i][j] += a.data[i][k] * temp;
        }
      }
    }
  }

  double end = omp_get_wtime();

#ifdef DEBUG
  printf("Parallel - kji - completed\n");
#endif

  return end - start;
}
