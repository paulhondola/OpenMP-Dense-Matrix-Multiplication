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
	@echo "Building serial loop..."
	$(CC) $(CFLAGS) $(SERIAL_LOOP_SRC) $(SRC) -o $(SERIAL_LOOP_TARGET)
	@echo "Building parallel loop..."
	$(CC) $(CFLAGS) $(PARALLEL_LOOP_SRC) $(SRC) -o $(PARALLEL_LOOP_TARGET)
	@echo "Building classic vs improved..."
	$(CC) $(CFLAGS) $(CLASSIC_VS_IMPROVED_SRC) $(SRC) -o $(CLASSIC_VS_IMPROVED_TARGET)
	@echo "Building tiled..."
	$(CC) $(CFLAGS) $(TILED_SRC) $(SRC) -o $(TILED_TARGET)
	@echo "Done building all executables."

serial_loop:
	@mkdir -p bin
	@echo "Building serial loop..."
	$(CC) $(CFLAGS) $(SERIAL_LOOP_SRC) $(SRC) -o $(SERIAL_LOOP_TARGET)
	@echo "Running serial loop..."
	./$(SERIAL_LOOP_TARGET)
	@echo "Done running serial loop."

parallel_loop:
	@mkdir -p bin
	@echo "Building parallel loop..."
	$(CC) $(CFLAGS) $(PARALLEL_LOOP_SRC) $(SRC) -o $(PARALLEL_LOOP_TARGET)
	@echo "Running parallel loop..."
	./$(PARALLEL_LOOP_TARGET)
	@echo "Done running parallel loop."

classic_vs_improved:
	@mkdir -p bin
	@echo "Building classic vs improved..."
	$(CC) $(CFLAGS) $(CLASSIC_VS_IMPROVED_SRC) $(SRC) -o $(CLASSIC_VS_IMPROVED_TARGET)
	@echo "Running classic vs improved..."
	./$(CLASSIC_VS_IMPROVED_TARGET)
	@echo "Done running classic vs improved."

tiled:
	@mkdir -p bin
	@echo "Building tiled..."
	$(CC) $(CFLAGS) $(TILED_SRC) $(SRC) -o $(TILED_TARGET)
	@echo "Running tiled..."
	./$(TILED_TARGET)
	@echo "Done running tiled."
	
all:
	@echo "Building and running all executables..."
	make serial_loop
	make parallel_loop
	make classic_vs_improved
	make tiled
	@echo "Done building and running all executables."

plot:
	@echo "Plotting benchmarks..."
	python3 -m py_compile benchmark/plot.py
	python3 benchmark/plot.py
	@echo "Done plotting benchmarks."