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

clear:
	rm -rf benchmark/data/*
	rm -rf benchmark/plots/*

build:
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SERIAL_LOOP_SRC) $(SRC) -o $(SERIAL_LOOP_TARGET)
	$(CC) $(CFLAGS) $(PARALLEL_LOOP_SRC) $(SRC) -o $(PARALLEL_LOOP_TARGET)
	$(CC) $(CFLAGS) $(CLASSIC_VS_IMPROVED_SRC) $(SRC) -o $(CLASSIC_VS_IMPROVED_TARGET)
	$(CC) $(CFLAGS) $(TILED_SRC) $(SRC) -o $(TILED_TARGET)

serial_loop:
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SERIAL_LOOP_SRC) $(SRC) -o $(SERIAL_LOOP_TARGET)
	./$(SERIAL_LOOP_TARGET)

parallel_loop:
	@mkdir -p bin
	$(CC) $(CFLAGS) $(PARALLEL_LOOP_SRC) $(SRC) -o $(PARALLEL_LOOP_TARGET)
	./$(PARALLEL_LOOP_TARGET)

classic_vs_improved:
	@mkdir -p bin
	$(CC) $(CFLAGS) $(CLASSIC_VS_IMPROVED_SRC) $(SRC) -o $(CLASSIC_VS_IMPROVED_TARGET)
	./$(CLASSIC_VS_IMPROVED_TARGET)

tiled:
	@mkdir -p bin
	$(CC) $(CFLAGS) $(TILED_SRC) $(SRC) -o $(TILED_TARGET)
	./$(TILED_TARGET)

all:
	make serial_loop
	make parallel_loop
	make classic_vs_improved
	make tiled

plot:
	python3 benchmark/plot.py
