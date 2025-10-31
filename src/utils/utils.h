// ABOUTME: Utility functions for data export and formatting
// ABOUTME: CSV writing and benchmarking data output helpers

#ifndef UTILS_H
#define UTILS_H

#include "../common/matrix.h"

typedef struct {
  char *header;
  char *filename;
} CSV_DATA;

extern CSV_DATA compare_serial_loop_permutations_data ;
extern CSV_DATA compare_parallel_loop_permutations_data;
extern CSV_DATA compare_classic_vs_improved_data;

FILE *open_csv_file(CSV_DATA csv_data);

#endif // UTILS_H
