// ABOUTME: Utility functions for data export and formatting
// ABOUTME: CSV writing and benchmarking data output helpers

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

CSV_DATA compare_serial_loop_permutations_data = {
    .header = "MATRIX_SIZE,STANDARD_TIME,IJK_TIME,IKJ_TIME,JIK_TIME,JKI_TIME,"
              "KIJ_TIME,KJI_TIME",
    .filename = "benchmark/data/compare_serial_loop_permutations.csv",
};

CSV_DATA compare_parallel_loop_permutations_data = {
    .header = "MATRIX_SIZE,THREADS,CHUNK,STANDARD_TIME,IJK_TIME,IKJ_TIME,JIK_"
              "TIME,JKI_TIME,KIJ_TIME,KJI_TIME",
    .filename = "benchmark/data/compare_parallel_loop_permutations.csv",
};

CSV_DATA compare_classic_vs_improved_data = {
    .header = "MATRIX_SIZE,THREADS,CHUNK,SERIAL_TIME,P2T,P4T,P8T,IMPROVED_"
              "SERIAL,IP2T,IP4T,IP8T",
    .filename = "benchmark/data/compare_classic_vs_improved.csv",
};

// if csv file is empty, add the header
FILE *open_csv_file(CSV_DATA csv_data) {
  FILE *csv_file = fopen(csv_data.filename, "a");

  if (csv_file == NULL) {
    printf("Error opening file %s\n", csv_data.filename);
    exit(1);
  }

  if (fseek(csv_file, 0, SEEK_END) == 0 && ftell(csv_file) == 0) {
    fprintf(csv_file, "%s\n", csv_data.header);
  }

  return csv_file;
}

// CSV saving functions will be implemented here
