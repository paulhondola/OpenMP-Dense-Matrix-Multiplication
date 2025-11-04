# OpenMP: Matrix Multiplication

## Overview

This project provides multiple implementations of dense square matrix multiplication, in both serial and OpenMP-parallel forms. It includes classic (i, j, k) loop permutations and a blocked/tiled variant, along with comprehensive validation and benchmarking tools with automated plotting.

### (i, j, k) loop permutations

Implements all six permutations of (i, j, k) for square matrices:

- **i-j-k** (classic): Row-major access for C, column-major for B
- **i-k-j**: Better cache locality for B
- **j-i-k, j-k-i, k-i-j, k-j-i**: Alternative loop orderings for comparison

The i-j-k and i-k-j variants are common baselines; all permutations are included for performance comparison.

### Blocked matrix multiplication

Includes a blocked (tiled) algorithm in serial and with OpenMP to improve cache locality and performance on large matrices.

- Handles matrix sizes not divisible by the block size
- Block size can be tuned empirically for a given machine
- Edge handling for partial blocks

### Dataset

- Square matrices N×N (typical N in the 1k–3k range for benchmarking)
- Values are generated at runtime with configurable seed (default: 42)
- Matrix size N is configurable via command-line arguments

### Validation and Benchmarking

Each version is validated by comparing against the classical i-j-k implementation using the benchmarking and validation utilities in `src/benchmark/`:

- **Serial validation**: All serial permutations validated against serial i-j-k
- **Parallel validation**: All parallel permutations validated against parallel i-j-k
- **Classic vs Improved**: Compares i-j-k vs i-k-j implementations
- **Tiled validation**: Validates blocked implementations

Results are exported to CSV files in `benchmark/data/` and can be visualized using the plotting script.

## Building and Running

The project uses a Makefile with the following commands:

### Build and Run Individual Benchmarks

- `make serial_loop` - Build and run serial loop permutations benchmark
- `make parallel_loop` - Build and run parallel loop permutations benchmark
- `make classic_vs_improved` - Build and run classic vs improved comparison
- `make tiled` - Build and run tiled/blocked benchmark

### Build All

- `make build` - Build all executables without running
- `make all` - Build and run all benchmarks sequentially

### Plotting

- `make plot` - Generate plots from benchmark data in `benchmark/data/`
  - Requires Python 3 with pandas and matplotlib
  - Plots are saved to `benchmark/plots/`

### Cleanup

- `make clear` - Clear benchmark data and plot files

**Recommended compiler**: gcc-15 with OpenMP support (`-fopenmp`).

**Compiler flags**: `-O3 -march=native -Wall -Wextra -fopenmp`

## Repository Structure

```text
OpenMP-Dense-Matrix-Multiplication/
├── benchmark/
│   ├── data/                   # CSV benchmark data files
│   │   ├── serial_permutations.csv
│   │   ├── parallel_permutations.csv
│   │   ├── classic_vs_improved.csv
│   │   └── tiled.csv
│   ├── plot.py                 # Python plotting script with robust file handling
│   └── plots/                  # Generated plot images (PNG)
│       ├── serial_permutations.png
│       ├── parallel_permutations.png
│       ├── classic_vs_improved.png
│       └── tiled.png
├── bin/                        # Compiled executables
│   ├── serial_loop.exe
│   ├── parallel_loop.exe
│   ├── classic_vs_improved.exe
│   └── tiled.exe
├── docs/
│   ├── DenseMatrix.pdf         # Problem description and theoretical background
│   └── omp_matrix_mult.c       # Reference implementation examples
├── src/
│   ├── benchmark/
│   │   ├── benchmark.c         # Benchmarking and validation functions
│   │   └── benchmark.h
│   ├── loop_permutations/
│   │   ├── parallel/
│   │   │   ├── mm_parallel.c   # OpenMP implementations for all 6 permutations
│   │   │   └── mm_parallel.h
│   │   └── serial/
│   │       ├── mm_serial.c     # Serial implementations for all 6 permutations
│   │       └── mm_serial.h
│   ├── main/                   # Entry point programs
│   │   ├── serial_loop.c       # Serial loop permutations benchmark
│   │   ├── parallel_loop.c     # Parallel loop permutations benchmark
│   │   ├── serial_vs_parallel.c # Classic vs improved comparison
│   │   ├── tiled.c             # Tiled/blocked benchmark
│   │   └── parameters.h       # Shared parameters and constants
│   ├── matrix/                 # Matrix utilities
│   │   ├── matrix.c            # Matrix allocation, initialization, validation
│   │   └── matrix.h
│   ├── tiled/
│   │   ├── parallel/
│   │   │   ├── mm_tiled_parallel.c  # OpenMP blocked/tiled implementation
│   │   │   └── mm_tiled_parallel.h
│   │   └── serial/
│   │       ├── mm_tiled_serial.c    # Serial blocked/tiled implementation
│   │       └── mm_tiled_serial.h
│   └── utils/
│       ├── utils.c             # CSV file handling utilities
│       └── utils.h
├── CLAUDE.md                   # Project documentation for AI assistants
├── Makefile                    # Build system
└── README.md
```

## Key Components

### Matrix Utilities (`src/matrix/`)

- Matrix allocation, initialization, and deallocation
- Random value generation with configurable seed
- Validation with epsilon comparison for floating-point values
- Helper functions used across all implementations

### Benchmarking (`src/benchmark/`)

- Validation functions comparing results against reference implementations
- Timing measurements using `omp_get_wtime()`
- Speedup calculations
- Test functions for serial/parallel permutations, classic vs improved, and tiled algorithms

### Loop Permutations (`src/loop_permutations/`)

- **Serial**: Baseline implementations without parallelization
- **Parallel**: OpenMP versions with configurable thread count and chunk size
- All six loop orderings (i-j-k, i-k-j, j-i-k, j-k-i, k-i-j, k-j-i)
- Function pointer arrays for dynamic selection

### Tiled Implementations (`src/tiled/`)

- Cache-blocked algorithms for improved performance
- Serial and parallel variants
- Tunable block size parameter

### Plotting (`benchmark/plot.py`)

- Python script for visualizing benchmark results
- Robust file handling with graceful error recovery
- Generates plots for:
  - **Serial loop permutations**: Speedup vs matrix size for all 6 loop orderings
  - **Parallel loop permutations**: Speedup vs matrix size with multiple lines per chunk size (chunk=1, 2, 4, 8, 16, etc.)
  - **Classic vs improved**: Comparison of i-j-k vs i-k-j implementations (serial and parallel with 2, 4, 8 threads)
  - **Tiled implementations**: Speedup vs matrix size with multiple lines per block size for each implementation (Serial IKJ, Parallel IKJ, Serial Tiled, Parallel Tiled)
- Automatically aggregates data when multiple measurements per matrix size exist
- Each plot uses different markers and linestyles to distinguish between parameter variations (chunk sizes, block sizes)
- Plots saved as high-resolution PNG files (300 DPI) in `benchmark/plots/`
