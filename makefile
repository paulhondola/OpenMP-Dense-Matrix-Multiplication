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
	@echo "BUILDING CLASSIC VS IMPROVED"
	$(CC) $(CFLAGS) $(CLASSIC_VS_IMPROVED_SRC) $(SRC) -o $(CLASSIC_VS_IMPROVED_TARGET)
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
	@echo "BUILDING CLASSIC VS IMPROVED"
	$(CC) $(CFLAGS_OPT) $(CLASSIC_VS_IMPROVED_SRC) $(SRC) -o $(CLASSIC_VS_IMPROVED_TARGET)
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

classic_vs_improved:
	@mkdir -p bin
	@echo "RUNNING CLASSIC VS IMPROVED"
	./$(CLASSIC_VS_IMPROVED_TARGET)
	@echo "TASK DONE"

tiled:
	@mkdir -p bin
	@echo "RUNNING TILED"
	./$(TILED_TARGET)
	@echo "TASK DONE"
	
all:
	make build
	@echo "RUNNING ALL EXECUTABLES"
	make serial_loop
	make parallel_loop
	make classic_vs_improved
	make tiled
	@echo "TASK DONE"

all_O3:
	make build_O3
	@echo "RUNNING ALL EXECUTABLES"
	make serial_loop
	make parallel_loop
	make classic_vs_improved
	make tiled
	@echo "TASK DONE"

plot:
	@echo "PLOTTING BENCHMARKS"
	python3 -m py_compile benchmark/plot.py
	python3 benchmark/plot.py
	@echo "TASK DONE"
