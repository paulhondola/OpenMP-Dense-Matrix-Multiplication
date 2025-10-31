// ABOUTME: Utility functions for data export and formatting
// ABOUTME: CSV writing and benchmarking data output helpers

#ifndef UTILS_H
#define UTILS_H

#include "../common/matrix.h"

typedef struct {
  char *header;
  char *filename;
} CSV_DATA;

extern CSV_DATA csv_serial_permutations;
extern CSV_DATA csv_parallel_permutations;
extern CSV_DATA csv_classic_vs_improved;

FILE *open_csv_file(CSV_DATA csv_data);

#endif // UTILS_H
