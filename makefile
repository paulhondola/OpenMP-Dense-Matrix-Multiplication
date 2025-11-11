SRC = $(shell find src -name "*.c" -type f ! -path "src/main/*")

PARALLEL_LOOP_SRC = src/main/parallel_loop.c
SERIAL_LOOP_SRC = src/main/serial_loop.c
SERIAL_PARALLEL_SCALING_SRC = src/main/serial-parallel-scaling.c
TILED_SRC = src/main/tiled.c

PARALLEL_LOOP_TARGET = bin/parallel_loop.exe
SERIAL_LOOP_TARGET = bin/serial_loop.exe
SERIAL_PARALLEL_SCALING_TARGET = bin/serial_parallel_scaling.exe
TILED_TARGET = bin/tiled.exe

CC = gcc-15
CFLAGS = -Wall -Wextra -fopenmp
CFLAGS_OPT = -O3 -march=native $(CFLAGS) 

clear:
	rm -rf benchmark/data/*
	rm -rf benchmark/plots/*

build:
	@mkdir -p bin
	@echo "OPTIMIZATION LEVEL 0"
	@echo "BUILDING SERIAL LOOP"
	$(CC) $(CFLAGS) $(SERIAL_LOOP_SRC) $(SRC) -o $(SERIAL_LOOP_TARGET)
	@echo "BUILDING PARALLEL LOOP"
	$(CC) $(CFLAGS) $(PARALLEL_LOOP_SRC) $(SRC) -o $(PARALLEL_LOOP_TARGET)
	@echo "BUILDING SERIAL PARALLEL SCALING"
	$(CC) $(CFLAGS) $(SERIAL_PARALLEL_SCALING_SRC) $(SRC) -o $(SERIAL_PARALLEL_SCALING_TARGET)
	@echo "BUILDING TILED"
	$(CC) $(CFLAGS) $(TILED_SRC) $(SRC) -o $(TILED_TARGET)
	@echo "TASK DONE"

build_O3:
	@mkdir -p bin
	@echo "OPTIMIZATION LEVEL 3 + NATIVE SIMD"
	@echo "BUILDING SERIAL LOOP"
	$(CC) $(CFLAGS_OPT) $(SERIAL_LOOP_SRC) $(SRC) -o $(SERIAL_LOOP_TARGET)
	@echo "BUILDING PARALLEL LOOP"
	$(CC) $(CFLAGS_OPT) $(PARALLEL_LOOP_SRC) $(SRC) -o $(PARALLEL_LOOP_TARGET)
	@echo "BUILDING SERIAL PARALLEL SCALING"
	$(CC) $(CFLAGS_OPT) $(SERIAL_PARALLEL_SCALING_SRC) $(SRC) -o $(SERIAL_PARALLEL_SCALING_TARGET)
	@echo "BUILDING TILED"
	$(CC) $(CFLAGS_OPT) $(TILED_SRC) $(SRC) -o $(TILED_TARGET)
	@echo "TASK DONE"

serial_loop:
	@mkdir -p bin
	@echo "RUNNING SERIAL LOOP"
	./$(SERIAL_LOOP_TARGET)
	@echo "TASK DONE"

parallel_loop:
	@mkdir -p bin
	@echo "RUNNING PARALLEL LOOP"
	./$(PARALLEL_LOOP_TARGET)
	@echo "TASK DONE"

serial_parallel_scaling:
	@mkdir -p bin
	@echo "RUNNING SERIAL PARALLEL SCALING"
	./$(SERIAL_PARALLEL_SCALING_TARGET)
	@echo "TASK DONE"

tiled:
	@mkdir -p bin
	@echo "RUNNING TILED"
	./$(TILED_TARGET)
	@echo "TASK DONE"
	
move_O0:
	@mkdir -p benchmark/data/O0
	@mkdir -p benchmark/plots/O0
	@if [ -f benchmark/data/serial_permutations.csv ]; then mv benchmark/data/serial_permutations.csv benchmark/data/O0/; fi
	@if [ -f benchmark/data/parallel_permutations.csv ]; then mv benchmark/data/parallel_permutations.csv benchmark/data/O0/; fi
	@if [ -f benchmark/data/serial_parallel_scaling_classic.csv ]; then mv benchmark/data/serial_parallel_scaling_classic.csv benchmark/data/O0/; fi
	@if [ -f benchmark/data/serial_parallel_scaling_improved.csv ]; then mv benchmark/data/serial_parallel_scaling_improved.csv benchmark/data/O0/; fi
	@if [ -f benchmark/data/tiled.csv ]; then mv benchmark/data/tiled.csv benchmark/data/O0/; fi
	@if [ -f benchmark/plots/serial_permutations.png ]; then mv benchmark/plots/serial_permutations.png benchmark/plots/O0/; fi
	@if [ -f benchmark/plots/parallel_permutations.png ]; then mv benchmark/plots/parallel_permutations.png benchmark/plots/O0/; fi
	@if [ -f benchmark/plots/serial_parallel_scaling_classic.png ]; then mv benchmark/plots/serial_parallel_scaling_classic.png benchmark/plots/O0/; fi
	@if [ -f benchmark/plots/serial_parallel_scaling_improved.png ]; then mv benchmark/plots/serial_parallel_scaling_improved.png benchmark/plots/O0/; fi
	@if [ -f benchmark/plots/tiled.png ]; then mv benchmark/plots/tiled.png benchmark/plots/O0/; fi
	@echo "Files moved to O0 subdirectories"

move_O3:
	@mkdir -p benchmark/data/O3
	@mkdir -p benchmark/plots/O3
	@if [ -f benchmark/data/serial_permutations.csv ]; then mv benchmark/data/serial_permutations.csv benchmark/data/O3/; fi
	@if [ -f benchmark/data/parallel_permutations.csv ]; then mv benchmark/data/parallel_permutations.csv benchmark/data/O3/; fi
	@if [ -f benchmark/data/serial_parallel_scaling_classic.csv ]; then mv benchmark/data/serial_parallel_scaling_classic.csv benchmark/data/O3/; fi
	@if [ -f benchmark/data/serial_parallel_scaling_improved.csv ]; then mv benchmark/data/serial_parallel_scaling_improved.csv benchmark/data/O3/; fi
	@if [ -f benchmark/data/tiled.csv ]; then mv benchmark/data/tiled.csv benchmark/data/O3/; fi
	@if [ -f benchmark/plots/serial_permutations.png ]; then mv benchmark/plots/serial_permutations.png benchmark/plots/O3/; fi
	@if [ -f benchmark/plots/parallel_permutations.png ]; then mv benchmark/plots/parallel_permutations.png benchmark/plots/O3/; fi
	@if [ -f benchmark/plots/serial_parallel_scaling_classic.png ]; then mv benchmark/plots/serial_parallel_scaling_classic.png benchmark/plots/O3/; fi
	@if [ -f benchmark/plots/serial_parallel_scaling_improved.png ]; then mv benchmark/plots/serial_parallel_scaling_improved.png benchmark/plots/O3/; fi
	@if [ -f benchmark/plots/tiled.png ]; then mv benchmark/plots/tiled.png benchmark/plots/O3/; fi
	@echo "Files moved to O3 subdirectories"

all: build serial_loop parallel_loop serial_parallel_scaling tiled

all_O3: build_O3 serial_loop parallel_loop serial_parallel_scaling tiled

plot:
	@echo "PLOTTING BENCHMARKS"
	python3 -m py_compile benchmark/src/main.py
	python3 benchmark/src/main.py
	@echo "TASK DONE"