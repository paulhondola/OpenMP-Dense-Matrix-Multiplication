// ABOUTME: Utility functions for data export and formatting
// ABOUTME: CSV writing and benchmarking data output helpers

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

CSV_DATA csv_serial_permutations = {
    .header = "MATRIX_SIZE,IJK_TIME,IKJ_TIME,JIK_TIME,JKI_TIME,"
              "KIJ_TIME,KJI_TIME",
    .filename = "benchmark/data/serial_permutations.csv",
};

CSV_DATA csv_parallel_permutations = {
    .header = "MATRIX_SIZE,THREADS,CHUNK,IJK_TIME,IKJ_TIME,JIK_"
              "TIME,JKI_TIME,KIJ_TIME,KJI_TIME",
    .filename = "benchmark/data/parallel_permutations.csv",
};

CSV_DATA csv_classic_vs_improved = {
    .header = "MATRIX_SIZE,CHUNK,SERIAL_TIME,P2T,P4T,P8T,IMPROVED_"
              "SERIAL,IP2T,IP4T,IP8T",
    .filename = "benchmark/data/classic_vs_improved.csv",
};

CSV_DATA csv_tiled = {
    .header = "MATRIX_SIZE,BLOCK_SIZE,SERIAL_TIME,P2T,P4T,P8T",
    .filename = "benchmark/data/tiled.csv",
};

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

void clear_csv_file(CSV_DATA csv_data) {
  FILE *csv_file = fopen(csv_data.filename, "w");
  if (csv_file == NULL) {
    printf("Error opening file %s\n", csv_data.filename);
    exit(1);
  }
  fclose(csv_file);
}