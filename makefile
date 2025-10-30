LOOP_SRC = src/loop_permutations/serial/mm_serial.c src/loop_permutations/parallel/mm_parallel.c
TILED_SRC = src/tiled/serial/mm_blocked_serial.c src/tiled/parallel/mm_blocked_omp.c
TEST_SRC = src/test/validate.c
COMMON_SRC = src/common/matrix.c

LOOP_TARGET = bin/loop_permutations
TILED_TARGET = bin/tiled

CC = gcc-15
CFLAGS = -O3 -march=native -Wall -Wextra -fopenmp

run_loop:
    $(CC) $(CFLAGS) $(LOOP_SRC) -o $(LOOP_TARGET)
    $(LOOP_TARGET)

run_tiled:
    $(CC) $(CFLAGS) $(TILED_SRC) -o $(TILED_TARGET)
    $(TILED_TARGET)
