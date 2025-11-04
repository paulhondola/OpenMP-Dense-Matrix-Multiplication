# Recursively find all .c files except in main directory
SRC = $(shell find src -name "*.c" -type f ! -path "src/main/*")

PARALLEL_LOOP_SRC = src/main/parallel_loop.c
SERIAL_LOOP_SRC = src/main/serial_loop.c
CLASSIC_VS_IMPROVED_SRC = src/main/serial_vs_parallel.c
TILED_SRC = src/main/tiled.c

PARALLEL_LOOP_TARGET = bin/parallel_loop.exe
SERIAL_LOOP_TARGET = bin/serial_loop.exe
CLASSIC_VS_IMPROVED_TARGET = bin/classic_vs_improved.exe
TILED_TARGET = bin/tiled.exe

CC = gcc-15
CFLAGS = -O3 -march=native -Wall -Wextra -fopenmp

MATRIX_SIZES = 500 750
CHUNK_SIZES = 1
THREAD_COUNT = 10
BLOCK_SIZE = 2 5 10

clear:
	rm -rf benchmark/data/*
	rm -rf benchmark/plots/*

serial_loop:
	$(CC) $(CFLAGS) $(SERIAL_LOOP_SRC) $(SRC) -o $(SERIAL_LOOP_TARGET)
	for matrix_size in $(MATRIX_SIZES); do \
		for chunk_size in $(CHUNK_SIZES); do \
			./$(SERIAL_LOOP_TARGET) $$matrix_size $(THREAD_COUNT) $$chunk_size; \
		done; \
	done

parallel_loop:
	$(CC) $(CFLAGS) $(PARALLEL_LOOP_SRC) $(SRC) -o $(PARALLEL_LOOP_TARGET)
	for matrix_size in $(MATRIX_SIZES); do \
		for thread_count in $(THREAD_COUNT); do \
			./$(PARALLEL_LOOP_TARGET) $$matrix_size $$thread_count $(CHUNK_SIZE); \
		done; \
	done

classic_vs_improved:
	$(CC) $(CFLAGS) $(CLASSIC_VS_IMPROVED_SRC) $(SRC) -o $(CLASSIC_VS_IMPROVED_TARGET)
	for matrix_size in $(MATRIX_SIZES); do \
		for chunk_size in $(CHUNK_SIZES); do \
			./$(CLASSIC_VS_IMPROVED_TARGET) $$matrix_size $$chunk_size; \
		done; \
	done

tiled:
	$(CC) $(CFLAGS) $(TILED_SRC) $(SRC) -o $(TILED_TARGET)
	for matrix_size in $(MATRIX_SIZES); do \
		for thread_count in $(THREAD_COUNT); do \
			for block_size in $(BLOCK_SIZE); do \
				./$(TILED_TARGET) $$matrix_size $$thread_count $$block_size; \
			done; \
		done; \
	done
