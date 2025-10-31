# Recursively find all .c files except in main directory
SRC = $(shell find src -name "*.c" -type f ! -path "src/main/*")

MAIN_SRC = src/main/main.c
LOOP_SRC = src/main/loop_permutations.c
TILED_SRC = src/main/tiled.c

MAIN_TARGET = bin/main.exe
LOOP_TARGET = bin/loop_permutations.exe
TILED_TARGET = bin/tiled.exe

CC = gcc-15
CFLAGS = -O3 -march=native -Wall -Wextra -fopenmp

main:
	$(CC) $(CFLAGS) $(MAIN_SRC) $(SRC) -o $(MAIN_TARGET)
	./$(MAIN_TARGET) 1000 10 100

loop:
	$(CC) $(CFLAGS) $(LOOP_SRC) $(SRC) -o $(LOOP_TARGET)
	./$(LOOP_TARGET) 1000 10 100

tiled:
	$(CC) $(CFLAGS) $(TILED_SRC) $(SRC) -o $(TILED_TARGET)
	./$(TILED_TARGET) 1000 10 100
