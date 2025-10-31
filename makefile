# Recursively find all .c files except in main directory
SRC = $(shell find src -name "*.c" -type f ! -path "src/main/*")

MAIN_TARGET = bin/main.exe
LOOP_TARGET = bin/loop_permutations.exe
TILED_TARGET = bin/tiled.exe

CC = gcc-15
CFLAGS = -O3 -march=native -Wall -Wextra -fopenmp

main:
	$(CC) $(CFLAGS) src/main/main.c $(SRC) -o $(MAIN_TARGET)
	./$(MAIN_TARGET)

loop:
	$(CC) $(CFLAGS) src/main/loop_permutations.c $(SRC) -o $(LOOP_TARGET)
	./$(LOOP_TARGET)

tiled:
	$(CC) $(CFLAGS) src/main/tiled.c $(SRC) -o $(TILED_TARGET)
	./$(TILED_TARGET)
