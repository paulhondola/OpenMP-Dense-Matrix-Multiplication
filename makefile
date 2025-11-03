# Recursively find all .c files except in main directory
SRC = $(shell find src -name "*.c" -type f ! -path "src/main/*")

MAIN_SRC = src/main/main.c
LOOP_SRC = src/main/loop_permutations.c
TILED_SRC = src/main/tiled.c

LOOP_TARGET = bin/loop_permutations.exe
TILED_TARGET = bin/tiled.exe

CC = gcc-15
CFLAGS = -O3 -march=native -Wall -Wextra -fopenmp

MATRIX_SIZES = 500 750 1000 1250 1500 1750 2000 2250 2500 2750 3000
CHUNK_SIZES = 1 10 25 50 75 100
THREAD_COUNT = 10

clear:
	rm -rf benchmark/data/*
	rm -rf benchmark/plots/*

loop:
	$(CC) $(CFLAGS) $(LOOP_SRC) $(SRC) -o $(LOOP_TARGET)
	for matrix_size in $(MATRIX_SIZES); do
		for chunk_size in $(CHUNK_SIZES); do
			./$(LOOP_TARGET) $$matrix_size $(THREAD_COUNT) $$chunk_size
		done
	done

loop_plot:
	python3 benchmark/plot.py

tiled:
	$(CC) $(CFLAGS) $(TILED_SRC) $(SRC) -o $(TILED_TARGET)
	./$(TILED_TARGET) 1000 10 100
