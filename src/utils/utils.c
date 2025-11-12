// ABOUTME: Utility functions for data export and formatting
// ABOUTME: CSV writing and benchmarking data output helpers

#include "utils.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static char *output_folder = NULL;

CSV_DATA csv_serial_permutations = {
    .header = "MATRIX_SIZE,IJK,IKJ,JIK,JKI,KIJ,KJI",
    .filename = "serial_permutations.csv",
};

CSV_DATA csv_parallel_permutations = {
    .header = "MATRIX_SIZE,THREADS,CHUNK,IJK,IKJ,JIK,JKI,KIJ,KJI",
    .filename = "parallel_permutations.csv",
};

CSV_DATA csv_serial_parallel_scaling_classic = {
    .header = "MATRIX_SIZE,CHUNK,SERIAL_BASELINE,P2T,P4T,P8T",
    .filename = "serial_parallel_scaling_classic.csv",
};

CSV_DATA csv_serial_parallel_scaling_improved = {
    .header = "MATRIX_SIZE,CHUNK,SERIAL_BASELINE,P2T,P4T,P8T",
    .filename = "serial_parallel_scaling_improved.csv",
};

CSV_DATA csv_tiled = {
    .header = "MATRIX_SIZE,THREADS,BLOCK_SIZE,SERIAL_IKJ,PARALLEL_IKJ,SERIAL_"
              "TILED,PARALLEL_TILED,PARALLEL_TILED_TASKS",
    .filename = "tiled.csv",
};

void set_output_folder(const char *folder_name) {
  if (output_folder != NULL) {
    free(output_folder);
    output_folder = NULL;
  }

  if (folder_name != NULL && strlen(folder_name) > 0) {
    output_folder = malloc(strlen(folder_name) + 1);
    if (output_folder == NULL) {
      printf("Error: Failed to allocate memory for output folder\n");
      exit(1);
    }
    strcpy(output_folder, folder_name);
  }
}

void ensure_directory_exists(const char *path) {
  struct stat st = {0};
  if (stat(path, &st) == -1) {
    // Directory doesn't exist, create it
    // Create parent directories recursively
    char *path_copy = malloc(strlen(path) + 1);
    if (path_copy == NULL) {
      printf("Error: Failed to allocate memory for path\n");
      exit(1);
    }
    strcpy(path_copy, path);

    // Create directories recursively
    for (char *p = path_copy + 1; *p; p++) {
      if (*p == '/') {
        *p = '\0';
        if (stat(path_copy, &st) == -1) {
          if (mkdir(path_copy, 0755) != 0) {
            printf("Error: Failed to create directory %s\n", path_copy);
            free(path_copy);
            exit(1);
          }
        }
        *p = '/';
      }
    }
    // Create the final directory
    if (stat(path_copy, &st) == -1) {
      if (mkdir(path_copy, 0755) != 0) {
        printf("Error: Failed to create directory %s\n", path_copy);
        free(path_copy);
        exit(1);
      }
    }
    free(path_copy);
  }
}

FILE *open_csv_file(CSV_DATA csv_data) {
  char full_path[PATH_MAX];

  // Construct the full path
  if (output_folder != NULL && strlen(output_folder) > 0) {
    snprintf(full_path, sizeof(full_path), "benchmark/data/%s/%s",
             output_folder, csv_data.filename);
  } else {
    snprintf(full_path, sizeof(full_path), "benchmark/data/%s",
             csv_data.filename);
  }

  // Ensure directory exists
  char dir_path[PATH_MAX];
  snprintf(dir_path, sizeof(dir_path), "%s", full_path);
  char *last_slash = strrchr(dir_path, '/');
  if (last_slash != NULL) {
    *last_slash = '\0';
    ensure_directory_exists(dir_path);
  }

  FILE *csv_file = fopen(full_path, "a");

  if (csv_file == NULL) {
    printf("Error opening file %s\n", full_path);
    exit(1);
  }

  if (fseek(csv_file, 0, SEEK_END) == 0 && ftell(csv_file) == 0) {
    fprintf(csv_file, "%s\n", csv_data.header);
  }

  return csv_file;
}

void clear_csv_file(CSV_DATA csv_data) {
  char full_path[PATH_MAX];

  // Construct the full path
  if (output_folder != NULL && strlen(output_folder) > 0) {
    snprintf(full_path, sizeof(full_path), "benchmark/data/%s/%s",
             output_folder, csv_data.filename);
  } else {
    snprintf(full_path, sizeof(full_path), "benchmark/data/%s",
             csv_data.filename);
  }

  FILE *csv_file = fopen(full_path, "w");
  if (csv_file == NULL) {
    printf("Error opening file %s\n", full_path);
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

  *matrix_size = atoi(argv[1]);
  *thread_count = atoi(argv[2]);
  *chunk_size = atoi(argv[3]);
}
