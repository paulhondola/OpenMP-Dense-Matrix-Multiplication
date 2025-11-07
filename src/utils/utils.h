// ABOUTME: Utility functions for data export and formatting
// ABOUTME: CSV writing and benchmarking data output helpers

#ifndef UTILS_H
#define UTILS_H

#include "../matrix/matrix.h"

typedef struct {
  char *header;
  char *filename;
} CSV_DATA;

extern CSV_DATA csv_serial_permutations;
extern CSV_DATA csv_parallel_permutations;
extern CSV_DATA csv_serial_parallel_scaling_classic;
extern CSV_DATA csv_serial_parallel_scaling_improved;
extern CSV_DATA csv_tiled;

FILE *open_csv_file(CSV_DATA);
void clear_csv_file(CSV_DATA);
void usage(char *);
void get_args(int argc, char *argv[], int *matrix_size, int *thread_count,
              int *chunk_size);

#endif // UTILS_H
