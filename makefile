LOOP_IMPL = src/loop_permutations/serial/mm_serial.c src/loop_permutations/parallel/mm_parallel.c
TILED_IMPL = src/tiled/serial/mm_blocked_serial.c src/tiled/parallel/mm_blocked_omp.c
TEST_SRC = src/test/test.c
COMMON_SRC = src/common/matrix.c

LOOP_TARGET = bin/loop_permutations.exe
TILED_TARGET = bin/tiled.exe
MAIN_TARGET = bin/main.exe

CC = gcc-15
CFLAGS = -O3 -march=native -Wall -Wextra -fopenmp

main:
	$(CC) $(CFLAGS) src/main/main.c $(COMMON_SRC) -o $(MAIN_TARGET)
	./$(MAIN_TARGET)

loop:
	$(CC) $(CFLAGS) src/main/loop_permutations.c $(LOOP_IMPL) $(TEST_SRC) $(COMMON_SRC) -o $(LOOP_TARGET)
	./$(LOOP_TARGET)

tiled:
	$(CC) $(CFLAGS) src/main/tiled.c $(TILED_IMPL) $(TEST_SRC) $(COMMON_SRC) -o $(TILED_TARGET)
	./$(TILED_TARGET)
