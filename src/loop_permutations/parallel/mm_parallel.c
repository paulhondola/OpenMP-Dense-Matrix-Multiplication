#include "mm_parallel.h"
#include <omp.h>

parallel_loop_permutation_function parallel_f[PERMUTATIONS] = {
    parallel_multiply_ijk, parallel_multiply_ikj, parallel_multiply_jik,
    parallel_multiply_jki, parallel_multiply_kij, parallel_multiply_kji,
};

double parallel_multiply_ijk(Matrix a, Matrix b, Matrix c, int thread_count,
                             int chunk) {
  matrix_fill_zero(c);
  int i, j, k;

  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k), shared(a, b, c, chunk)
  {
#pragma omp for schedule(static, chunk)
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        for (k = 0; k < N; k++) {
          c[i][j] += a[i][k] * b[k][j];
        }
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
    for (i = 0; i < N; i++) {
      for (k = 0; k < N; k++) {
        temp = a[i][k];
        for (j = 0; j < N; j++) {
          c[i][j] += temp * b[k][j];
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

  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(i, j, k), shared(a, b, c, chunk)
  {
#pragma omp for schedule(static, chunk)
    for (j = 0; j < N; j++) {
      for (i = 0; i < N; i++) {
        for (k = 0; k < N; k++) {
          c[i][j] += a[i][k] * b[k][j];
        }
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
    for (j = 0; j < N; j++) {
      for (k = 0; k < N; k++) {
        temp = b[k][j];
        for (i = 0; i < N; i++) {
          c[i][j] += a[i][k] * temp;
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
    for (k = 0; k < N; k++) {
#pragma omp for schedule(static, chunk)
      for (i = 0; i < N; i++) {
        temp = a[i][k];
        for (j = 0; j < N; j++) {
          c[i][j] += temp * b[k][j];
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
    for (k = 0; k < N; k++) {
#pragma omp for schedule(static, chunk)
      for (j = 0; j < N; j++) {
        temp = b[k][j];
        for (i = 0; i < N; i++) {
          c[i][j] += a[i][k] * temp;
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
