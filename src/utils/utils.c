// ABOUTME: Utility functions for data export and formatting
// ABOUTME: CSV writing and benchmarking data output helpers

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

CSV_DATA csv_serial_permutations = {
    .header = "MATRIX_SIZE,IJK_SPEEDUP,IKJ_SPEEDUP,JIK_SPEEDUP,JKI_SPEEDUP,"
              "KIJ_SPEEDUP,KJI_SPEEDUP",
    .filename = "benchmark/data/serial_permutations.csv",
};

CSV_DATA csv_parallel_permutations = {
    .header = "MATRIX_SIZE,THREADS,CHUNK,IJK_SPEEDUP,IKJ_SPEEDUP,JIK_"
              "SPEEDUP,JKI_SPEEDUP,KIJ_SPEEDUP,KJI_SPEEDUP",
    .filename = "benchmark/data/parallel_permutations.csv",
};

CSV_DATA csv_classic_vs_improved = {
    .header = "MATRIX_SIZE,CHUNK,SERIAL_BASELINE,P2T_SPEEDUP,P4T_SPEEDUP,P8T_"
              "SPEEDUP,IMPROVED_SERIAL_SPEEDUP,IP2T_SPEEDUP,IP4T_SPEEDUP,IP8T_"
              "SPEEDUP",
    .filename = "benchmark/data/classic_vs_improved.csv",
};

CSV_DATA csv_tiled = {
    .header = "MATRIX_SIZE,BLOCK_SIZE,SERIAL_BASELINE,P2T_SPEEDUP,P4T_SPEEDUP,"
              "P8T_SPEEDUP",
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

void usage(char *program_name) {
  printf("Usage: %s <matrix_size> <thread_count> <chunk_size>\n", program_name);
  printf("Chunk: The chunk size for the parallel loop permutations\n");
  exit(1);
}

void get_args(int argc, char *argv[], int *matrix_size, int *thread_count,
              int *chunk_size) {
  if (argc != 4) {
    usage(argv[0]);
  }

  if (argc != 4) {
    usage(argv[0]);
  }

  *matrix_size = atoi(argv[1]);
  *thread_count = atoi(argv[2]);
  *chunk_size = atoi(argv[3]);
}
