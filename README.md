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

- Square matrices N×N (default sizes: 480, 960, 1920)
- Values are generated at runtime with configurable seed (default: 42)
- Random values range from -10 to 10 (configurable in `src/main/parameters.h`)

### Validation and Benchmarking

Each version is validated by comparing against the classical i-j-k implementation using the benchmarking and validation utilities in `src/benchmark/`:

- **Serial validation**: All serial permutations validated against serial i-j-k
- **Parallel validation**: All parallel permutations validated against parallel i-j-k
- **Classic vs Improved**: Compares i-j-k vs i-k-j implementations
- **Tiled validation**: Validates blocked implementations

Results are exported to CSV files in `benchmark/data/` and can be visualized using the plotting script.

## Building and Running

The project uses a Makefile with the following commands:

### Run Individual Benchmarks

**Note**: These targets run the executables. Build them first using `make build` or `make build_O3`.

- `make serial_loop` - Run serial loop permutations benchmark
- `make parallel_loop` - Run parallel loop permutations benchmark
- `make serial_parallel_scaling` - Run serial vs parallel scaling comparison (classic and improved)
- `make tiled` - Run tiled/blocked benchmark

### Build Targets

- `make build` - Build all executables without optimization (`-Wall -Wextra -fopenmp`)
- `make build_O3` - Build all executables with optimization (`-O3 -march=native -Wall -Wextra -fopenmp`)
- `make all` - Build (without optimization) and run all benchmarks sequentially
- `make all_O3` - Build (with optimization) and run all benchmarks sequentially

### Plotting

- `make plot` - Generate all plots from benchmark data in `benchmark/data/`
  - Requires Python 3 with pandas and matplotlib
  - Uses the wrapper script `benchmark/src/main.py` which calls individual plot scripts
  - Individual scripts can also be run directly from `benchmark/src/`
  - Plots are saved to `benchmark/plots/` as PNG files (300 DPI)

### Cleanup

- `make clear` - Clear benchmark data and plot files

**Recommended compiler**: gcc-15 with OpenMP support (`-fopenmp`).

**Compiler flags**:

- Basic build: `-Wall -Wextra -fopenmp`
- Optimized build: `-O3 -march=native -Wall -Wextra -fopenmp`

## Repository Structure

```text
OpenMP-Dense-Matrix-Multiplication/
├── benchmark/
│   ├── data/                   # CSV benchmark data files
│   │   ├── serial_permutations.csv
│   │   ├── parallel_permutations.csv
│   │   ├── serial_parallel_scaling_classic.csv
│   │   ├── serial_parallel_scaling_improved.csv
│   │   └── tiled.csv
│   ├── plots/                  # Generated plot images (PNG)
│   │   ├── serial_permutations.png
│   │   ├── parallel_permutations.png
│   │   ├── serial_parallel_scaling_classic.png
│   │   ├── serial_parallel_scaling_improved.png
│   │   └── tiled.png
│   └── src/                    # Individual plot generation scripts
│       ├── main.py             # Main script that calls all individual plot scripts
│       ├── plot_serial_permutations.py
│       ├── plot_parallel_permutations.py
│       ├── plot_serial_parallel_scaling_classic.py
│       ├── plot_serial_parallel_scaling_improved.py
│       ├── plot_tiled.py
│       └── utils.py            # Common utilities (CSV loading, aggregation, directory helpers)
├── bin/                        # Compiled executables
│   ├── serial_loop.exe
│   ├── parallel_loop.exe
│   ├── serial_parallel_scaling.exe
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
│   │   ├── serial-parallel-scaling.c # Serial vs parallel scaling comparison
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

### Configuration (`src/main/parameters.h`)

Centralized configuration for all benchmarks:

- **Matrix sizes**: Default `{480, 960, 1920}` (configurable via `MATRIX_SIZES`)
- **Thread count**: Default `10` (configurable via `THREAD_COUNT`)
- **Chunk sizes**: Default `{25, 50, 100, 150}` for parallel scheduling (configurable via `CHUNK_SIZES`)
- **Block sizes**: Default `{2, 8, 16, 32, 48}` for tiled multiplication (configurable via `BLOCK_SIZES`)
- **Random seed**: Default `42` (configurable via `SEED`)
- **Validation tolerance**: Default `1e-6` for floating-point comparison (configurable via `EPSILON`)
- **Debug flags**: `DEBUG` and `DEBUG_MATRIX` for verbose output

### Plotting Scripts (`benchmark/src/`)

Modular plotting system with individual scripts for each plot type:

- **Modular design**: Five separate Python scripts, one per plot type:
  - `plot_serial_permutations.py` - Serial loop permutations: Speedup vs matrix size for all 6 loop orderings
  - `plot_parallel_permutations.py` - Parallel loop permutations: Speedup vs matrix size with separate lines per chunk size
  - `plot_serial_parallel_scaling_classic.py` - Classic (i-j-k): Serial vs parallel scaling with 2, 4, 8 threads
  - `plot_serial_parallel_scaling_improved.py` - Improved (i-k-j): Serial vs parallel scaling with 2, 4, 8 threads
  - `plot_tiled.py` - Tiled implementations: Speedup vs matrix size with separate lines per block size
- **Common utilities** (`utils.py`): Shared functions for CSV loading, data aggregation, and directory management
- **Wrapper script** (`main.py`): Convenience script that runs all individual plot scripts sequentially
- **Features**:
  - Automatically aggregates duplicate measurements (averages when multiple runs per matrix size exist)
  - Uses distinct markers and line styles for parameter variations (chunk sizes, block sizes)
  - Plots saved as high-resolution PNG files in `benchmark/plots/`
