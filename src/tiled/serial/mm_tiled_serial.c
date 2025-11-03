#include "mm_tiled_serial.h"
#include <omp.h>

double serial_multiply_tiled(Matrix a, Matrix b, Matrix c, int block_size) {
  int n = a.size;
  double temp = 0;

  double start = omp_get_wtime();
  for (int jj = 0; jj < n; jj += block_size) {
    for (int kk = 0; kk < n; kk += block_size) {
      for (int i = 0; i < n; i++) {
        for (int j = jj; j < ((jj + block_size) > n ? n : (jj + block_size));
             j++) {
          temp = 0;
          for (int k = kk; k < ((kk + block_size) > n ? n : (kk + block_size));
               k++) {
            temp += a.data[i][k] * b.data[k][j];
          }
          c.data[i][j] += temp;
        }
      }
    }
  }

  double result = omp_get_wtime() - start;

#ifdef DEBUG
  printf("Serial - tiled - block size %d - completed - time: %f\n", block_size,
         result);
#endif

  return result;
}
