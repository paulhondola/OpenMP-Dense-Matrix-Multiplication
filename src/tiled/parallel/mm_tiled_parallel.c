#include "mm_tiled_parallel.h"
#include <omp.h>

double parallel_multiply_tiled(Matrix a, Matrix b, Matrix c, int thread_count,
                               int block_size) {
  matrix_fill_zero(c);
  int n = a.size;
  int block_i, block_j, block_k, i, j, k;
  double temp;

  double start = omp_get_wtime();

#pragma omp parallel num_threads(thread_count), default(none),                 \
    private(temp, block_i, block_j, block_k, i, j, k),                         \
    shared(a, b, c, block_size, n)
  {
#pragma omp single
    for (block_i = 0; block_i < n; block_i += block_size) {
      for (block_j = 0; block_j < n; block_j += block_size) {
        for (block_k = 0; block_k < n; block_k += block_size) {
          int i_end = (block_i + block_size > n) ? n : (block_i + block_size);
          int j_end = (block_j + block_size > n) ? n : (block_j + block_size);
          int k_end = (block_k + block_size > n) ? n : (block_k + block_size);
#pragma omp task
          for (i = block_i; i < i_end; i++) {
            for (j = block_j; j < j_end; j++) {
              temp = 0;
              for (k = block_k; k < k_end; k++) {
                temp += a.data[i][k] * b.data[k][j];
              }
#pragma omp atomic
              c.data[i][j] += temp;
            }
          }
        }
      }
    }
  }
  return omp_get_wtime() - start;
}
