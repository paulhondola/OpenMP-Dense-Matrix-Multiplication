# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Dense square matrix multiplication implementations in C using OpenMP. The project explores performance optimization through:

1. **Loop permutations**: All six permutations of (i,j,k) loops
2. **Blocked/tiled algorithms**: Cache-friendly tiled multiplication

## Build System

The project uses a Makefile with the following commands:

```bash
# Build and run individual benchmarks
make serial_loop                # Serial loop permutations
make parallel_loop              # Parallel loop permutations
make serial_parallel_scaling    # Compare i-j-k vs i-k-j (serial and parallel scaling)
make tiled                      # Tiled/blocked implementations

# Build all without running
make build

# Build and run all benchmarks sequentially
make all

# Generate plots from benchmark data
make plot

# Clean benchmark data and plots
make clear
```

**Compiler requirements:**

- gcc-15 with OpenMP support
- Basic build flags: `-Wall -Wextra -fopenmp`
- Optimized build flags: `-O3 -march=native -Wall -Wextra -fopenmp`
- Use `make build` for basic build or `make build_O3` for optimized build
- Output binaries go to `bin/` directory
- Entry points: `src/main/serial_loop.c`, `src/main/parallel_loop.c`, `src/main/serial-parallel-scaling.c`, `src/main/tiled.c`

## Architecture

### Core Components

**Configuration** (`src/main/parameters.h`):

- Centralized compile-time configuration for all benchmarks
- `MATRIX_SIZES`: Array of matrix sizes to test (default: 480, 960, 1920)
- `THREAD_COUNT`: Number of OpenMP threads (default: 10)
- `CHUNK_SIZES`: Array of chunk sizes for parallel scheduling (default: 25, 50, 100, 150)
- `BLOCK_SIZES`: Array of block sizes for tiled multiplication (default: 2, 8, 16, 32, 48)
- `SEED`: Random number generation seed (default: 42)
- `EPSILON`: Validation tolerance for floating-point comparison (default: 1e-6)
- `UNIFORM_MIN`, `UNIFORM_MAX`: Range for random matrix values (default: -10 to 10)
- Debug flags: `DEBUG` and `DEBUG_MATRIX`

**Matrix utilities** (`src/matrix/matrix.{c,h}`):

- Matrix allocation, initialization, deallocation
- Random value generation with configurable seed
- Helper functions used across all implementations
- Uses `Matrix` struct with `double** data` and `int size` fields
- Function typedefs: `serial_loop_benchmark` and `parallel_loop_benchmark` for function pointer arrays

**Benchmarking and validation** (`src/benchmark/benchmark.{c,h}`):

- Compares results against reference i-j-k implementation
- Validation uses `validate()` function from `matrix.c` with epsilon comparison (configured in `parameters.h`)
- `test_serial_loop_permutations()`: Validates all serial permutations against serial i-j-k, returns timing results
- `test_parallel_loop_permutations()`: Validates all parallel permutations against parallel i-j-k, returns timing results
- `test_serial_parallel_scaling_classic()`: Tests i-j-k implementation (serial and parallel with 2, 4, 8 threads)
- `test_serial_parallel_scaling_improved()`: Tests i-k-j implementation (serial and parallel with 2, 4, 8 threads)
- `test_tiled()`: Validates tiled implementations against i-k-j baseline
- `compute_speedup()`: Calculates speedup relative to baseline (first element in time_results array)

**Utilities** (`src/utils/utils.{c,h}`):

- CSV file handling for benchmark data export
- Functions to write benchmark results to CSV files in `benchmark/data/`
- Output files: `serial_permutations.csv`, `parallel_permutations.csv`, `serial_parallel_scaling_classic.csv`, `serial_parallel_scaling_improved.csv`, `tiled.csv`

### Loop Permutation Implementations

Location: `src/loop_permutations/`

Six permutations for each variant (serial and parallel):

- **i-j-k** (classic): Row-major access for C, column-major for B
- **i-k-j**: Better cache locality for B
- **j-i-k**: Interchanges outer loops
- **j-k-i**: Different cache pattern
- **k-i-j**: K outer loop, updates entire C
- **k-j-i**: K outer loop variant

**Serial** (`serial/mm_serial.{c,h}`):

- Baseline implementations without parallelization
- Functions: `serial_multiply_ijk()`, `serial_multiply_ikj()`, `serial_multiply_jik()`, `serial_multiply_jki()`, `serial_multiply_kij()`, `serial_multiply_kji()`
- All functions return execution time in seconds (using `omp_get_wtime()`)
- Accessed via `serial_f[]` function pointer array indexed 0-5

**Parallel** (`parallel/mm_parallel.{c,h}`):

- OpenMP versions with `#pragma omp for`
- Functions: `parallel_multiply_ijk()`, `parallel_multiply_ikj()`, `parallel_multiply_jik()`, `parallel_multiply_jki()`, `parallel_multiply_kij()`, `parallel_multiply_kji()`
- All functions accept `thread_count` and `chunk` parameters
- All functions return execution time in seconds
- Accessed via `parallel_f[]` function pointer array indexed 0-5

**Function pointer pattern**: Both serial and parallel implementations use function pointer arrays (`serial_loop_permutation_function` and `parallel_loop_permutation_function` typedefs) to allow dynamic selection of loop permutations at runtime via array indexing

Reference: `docs/omp_matrix_mult.c` shows example i-j-k and i-k-j implementations

### Tiled/Blocked Implementations

Location: `src/tiled/`

**Serial** (`serial/mm_tiled_serial.{c,h}`): Cache-blocked i-k-j algorithm
**Parallel** (`parallel/mm_tiled_parallel.{c,h}`): OpenMP parallel blocked version

Key considerations:

- Block size is a tunable parameter (affects cache performance)
- Must handle matrices where N is not divisible by block size
- Edge handling for partial blocks is critical
- Both implementations use i-k-j ordering (better cache locality than i-j-k)

### Entry Points

All entry points read configuration from `src/main/parameters.h` and output CSV files to `benchmark/data/`.

**`src/main/serial_loop.c`**:

- Benchmarks all six serial loop permutations
- Tests all matrix sizes defined in `MATRIX_SIZES`
- Calls `test_serial_loop_permutations()` for validation and timing
- Writes results to `benchmark/data/serial_permutations.csv`

**`src/main/parallel_loop.c`**:

- Benchmarks all six parallel loop permutations
- Tests all matrix sizes and chunk sizes from parameters
- Calls `test_parallel_loop_permutations()` for validation and timing
- Writes results to `benchmark/data/parallel_permutations.csv`

**`src/main/serial-parallel-scaling.c`**:

- Compares i-j-k vs i-k-j implementations (serial and parallel with 2, 4, 8 threads)
- Tests all matrix sizes and chunk sizes from parameters
- Calls `test_serial_parallel_scaling_classic()` and `test_serial_parallel_scaling_improved()` for validation and timing
- Writes results to TWO separate CSV files:
  - `benchmark/data/serial_parallel_scaling_classic.csv` (for i-j-k)
  - `benchmark/data/serial_parallel_scaling_improved.csv` (for i-k-j)

**`src/main/tiled.c`**:

- Benchmarks tiled implementations with various block sizes
- Tests serial i-k-j, parallel i-k-j, serial tiled, and parallel tiled
- Calls `test_tiled()` for validation and timing
- Writes results to `benchmark/data/tiled.csv`

## Benchmarking

**Data flow:**

1. Configure parameters in `src/main/parameters.h` (matrix sizes, thread counts, chunk sizes, block sizes)
2. Build executables using `make build` (unoptimized) or `make build_O3` (optimized with -O3 -march=native)
3. Run `make all` (unoptimized) or `make all_O3` (optimized) to execute all benchmarks, or run individual targets
4. Benchmark data written to CSV files in `benchmark/data/`
5. Run `make plot` to generate visualizations with Python
6. Plots saved to `benchmark/plots/` as PNG files (300 DPI)

**Plotting** (`benchmark/src/`):

- Modular Python plotting system with individual scripts for each plot type
- Main entry point: `benchmark/src/main.py` (called by `make plot`)
- Individual plotting scripts:
  - `plot_serial_permutations.py`: Time vs matrix size for all 6 loop orderings
  - `plot_parallel_permutations.py`: Speedup vs matrix size with separate lines per chunk size
  - `plot_serial_parallel_scaling_classic.py`: i-j-k scaling (serial and parallel with 2, 4, 8 threads)
  - `plot_serial_parallel_scaling_improved.py`: i-k-j scaling (serial and parallel with 2, 4, 8 threads)
  - `plot_tiled.py`: Speedup vs matrix size with separate lines per block size
- Shared utilities in `utils.py` for CSV loading and data aggregation
- Robust error handling for missing or malformed CSV files
- Automatically aggregates duplicate measurements (averages when multiple runs per matrix size exist)
- Uses distinct markers and line styles for parameter variations
- Plots saved as 300 DPI PNG files in `benchmark/plots/`

## Development Notes

**OpenMP patterns from reference implementation:**

```c
#pragma omp parallel num_threads(nthreads), default(none), \
    private(i, j, k), shared(a, b, c, chunk)
#pragma omp for schedule(static, chunk)
```

**Key points:**

- Use `default(none)` to force explicit variable scoping
- Properly declare private vs shared variables
- Schedule type (static/dynamic) and chunk size affect performance
- Use `omp_get_wtime()` for accurate timing measurements
- Validation: Use epsilon comparison for floating-point results (configured in `parameters.h`)
- All timing measurements are in seconds

**Matrix storage:**

- This codebase uses a `Matrix` struct with `double** data` and `int size` fields
- Accessed as `matrix.data[i][j]` or `matrix.size`
- Must use `matrix_create()` to allocate and `matrix_destroy()` to deallocate
- `matrix_fill_random()` populates with random values in range [`UNIFORM_MIN`, `UNIFORM_MAX`]
- `matrix_fill_zero()` initializes to zeros
- Reference implementation (`docs/omp_matrix_mult.c`) uses static 2D arrays `double a[N][N]`

**Configuration and debugging:**

- All benchmark parameters centralized in `src/main/parameters.h`
- Define `DEBUG` for colored validation output (green/red for pass/fail)
- Define `DEBUG_MATRIX` for matrix value printing (useful for small test cases)
- Default matrix sizes: 480, 960, 1920
- Modify `parameters.h` and rebuild to change test configurations
- Note: `DEBUG` is enabled by default in `parameters.h`

## Documentation

- `docs/DenseMatrix.pdf`: Problem description and theoretical background
- `docs/omp_matrix_mult.c`: Reference implementation showing two versions (i-j-k and i-k-j) with both serial and parallel variants
