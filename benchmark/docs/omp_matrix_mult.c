/*
 * FILE: omp_matrix_mult.c
 * Matrix-Matrix multiplication
 * V1: classic algorithm, serial and parallel
 * V2: (ikj) algorithm, serial and parallel
 */

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EPSILON 0.000001

// #define DEBUG

#define N 1000 /* number of rows and column of matrices */

double a[N][N], /* matrix A to be multiplied */
    b[N][N],    /* matrix B to be multiplied */
    c[N][N],    /* result matrix C serial */
    c2[N][N];   /* result matrix C parallel */

/*
matrix multiplication - classical serial version
input: global vars a, b
output: global var c
*/
void serial_multiply(void) {
  int i, j, k;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
      c[i][j] = 0;
      for (k = 0; k < N; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
}

/*
matrix multiplication serial version (ikj)
input: global vars a, b
output: global var c
*/
void serial_multiply_v2(void) {
  int i, j, k;
  double temp, aik;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      c[i][j] = 0;

  for (i = 0; i < N; i++)
    for (k = 0; k < N; k++) {
      aik = a[i][k];
      for (int j = 0; j < N; j++) {
        c[i][j] += aik * b[k][j];
      }
    }
}

/*
matrix multiplication parallel version
input: global vars a, b
output: global var c2
*/
void parallel_multiply_v1(int nthreads, int chunk) {
  int i, j, k;
  double temp;
#pragma omp parallel num_threads(nthreads), default(none),                     \
    private(i, j, k, temp), shared(a, b, c2, chunk)
  {
#pragma omp for schedule(static, chunk)
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++) {
        c2[i][j] = 0;
        for (k = 0; k < N; k++)
          c2[i][j] += a[i][k] * b[k][j];
        // c2[i][j] = temp;
      }
  }
}

/*
matrix multiplication parallel version (ikj)
input: global vars a, b
output: global var c2
*/
void parallel_multiply_v2(int nthreads, int chunk) {
  int i, j, k;
  double temp, aik;
#pragma omp parallel num_threads(nthreads), default(none), private(i, j),      \
    shared(c2, chunk)
#pragma omp for schedule(static, chunk)
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      c2[i][j] = 0;

#pragma omp parallel num_threads(nthreads), default(none),                     \
    private(i, j, k, aik), shared(a, b, c2, chunk)
  {
#pragma omp for schedule(static, chunk)
    for (i = 0; i < N; i++)
      for (k = 0; k < N; k++) {
        aik = a[i][k];
        for (int j = 0; j < N; j++) {
          c2[i][j] += aik * b[k][j];
        }
      }
  }
}

void Generate_matrix(char *prompt, double mat[N][N]);
int Equal_matrixes(double mat1[N][N], double mat2[N][N]);
void Print_matrix(char *prompt, double mat[N][N]);

int main(int argc, char *argv[]) {
  int nthreads, chunk;
  double start, end, time_serial, time_parallel;

  nthreads = 4; /* set number of threads */
  chunk = 10;   /* set loop iteration chunk size */

  printf("--->Matrix size N=%d \n", N);

  Generate_matrix("Generating matrix a ...", a);

  Generate_matrix("Generating matrix b ...", b);

  printf("Start working serial V1 ... \n");
  start = omp_get_wtime();
  serial_multiply();
  end = omp_get_wtime();
  time_serial = end - start;

#ifdef DEBUG
  Print_matrix("Serial result ...", c);
#endif
  printf("Serial time V1 %lf seconds \n", time_serial);

  printf("Start working parallel V1 with %d threads ... \n", nthreads);
  start = omp_get_wtime();
  parallel_multiply_v1(nthreads, chunk);
  end = omp_get_wtime();
  time_parallel = (end - start);

#ifdef DEBUG
  Print_matrix("Parallel result V1 ...", c2);
#endif

  printf("Parallel time V1 %lf seconds \n", time_parallel);
  printf("Speedup = %2.2lf\n", time_serial / time_parallel);
  if (!Equal_matrixes(c, c2))
    printf("Attention! Serial and Parallel V1 Result not the same ! \n");

  printf("\n*********** V2\n");

  printf("Start working serial v2 ... \n");
  start = omp_get_wtime();
  serial_multiply_v2();
  end = omp_get_wtime();
  time_serial = end - start;

#ifdef DEBUG
  Print_matrix("Serial result V2 ...", c);
#endif
  printf("Serial time v2 %lf seconds \n", time_serial);

  printf("Start working parallel V2 with %d threads ... \n", nthreads);
  start = omp_get_wtime();
  parallel_multiply_v2(nthreads, chunk);
  end = omp_get_wtime();
  time_parallel = (end - start);

#ifdef DEBUG
  Print_matrix("Parallel result V2...", c3, N, N);
#endif

  printf("Parallel time V2 %lf seconds \n", time_parallel);
  printf("Speedup = %2.2lf\n", time_serial / time_parallel);

  if (!Equal_matrixes(c, c2))
    printf("Attention! Serial and Parallel V2 Result not the same ! \n");
}

void Generate_matrix(char *prompt, double mat[N][N]) {
  int i, j;

  srand(time(NULL));
  printf("%s\n", prompt);
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      mat[i][j] = rand();
}

void Print_matrix(char *title, double mat[N][N]) {
  int i, j;

  printf("%s\n", title);
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++)
      printf("%4.1f ", mat[i][j]);
    printf("\n");
  }
}

int Equal_matrixes(double mat1[N][N], double mat2[N][N]) {
  int i, j;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++)
      if (fabs(mat1[i][j] - mat2[i][j] > EPSILON)) {
        return 0;
      }
  }
  return 1;
}