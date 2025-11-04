#include "mm_tiled_serial.h"
#include "../../main/parameters.h"
#include <omp.h>

double serial_multiply_tiled(Matrix a, Matrix b, Matrix c, int block_size) {
  matrix_fill_zero(c);
  int n = a.size;
  double temp = 0;

  double start = omp_get_wtime();

  for (int block_i = 0; block_i < n; block_i += block_size) {
    for (int block_j = 0; block_j < n; block_j += block_size) {
      for (int block_k = 0; block_k < n; block_k += block_size) {
        int i_end = (block_i + block_size > n) ? n : (block_i + block_size);
        int j_end = (block_j + block_size > n) ? n : (block_j + block_size);
        int k_end = (block_k + block_size > n) ? n : (block_k + block_size);

        for (int i = block_i; i < i_end; i++) {
          for (int j = block_j; j < j_end; j++) {
            temp = 0;
            for (int k = block_k; k < k_end; k++) {
              temp += a.data[i][k] * b.data[k][j];
            }
            c.data[i][j] += temp;
          }
        }
      }
    }
  }

  return omp_get_wtime() - start;
}
