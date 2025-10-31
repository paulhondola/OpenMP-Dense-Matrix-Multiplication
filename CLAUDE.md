# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Dense square matrix multiplication implementations in C using OpenMP. The project explores performance optimization through:

1. **Loop permutations**: All six permutations of (i,j,k) loops
2. **Blocked/tiled algorithms**: Cache-friendly tiled multiplication

## Build System

The project uses a Makefile with the following commands:

```bash
# Build and run main entry point
make main

# Build and run loop permutations benchmark
make loop

# Build and run tiled/blocked benchmark
make tiled
```

**Compiler requirements:**

- gcc-15 with OpenMP support
- Compiler flags: `-O3 -march=native -Wall -Wextra -fopenmp`
- Output binaries go to `bin/` directory
- Entry points: `src/main/main.c`, `src/main/loop_permutations.c`, and `src/main/tiled.c`

## Architecture

### Core Components

**Matrix utilities** (`src/common/matrix.{c,h}`):

- Matrix allocation, initialization, deallocation
- Random value generation with configurable seed (default: `SEED = 42`)
- Helper functions used across all implementations
- Uses `Matrix` typedef for `double**` pointer-to-pointer representation
- Default matrix size: `N = 1000` (compile-time constant)

**Validation and testing** (`src/test/test.{c,h}`):

- Compares results against reference i-j-k implementation
- Uses epsilon comparison for floating-point values (`EPSILON = 0.000001`)
- `test_serial_loop_permutations()`: Validates all serial permutations against serial i-j-k
- `test_parallel_loop_permutations()`: Validates all parallel permutations against parallel i-j-k

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

**Parallel** (`parallel/mm_parallel.{c,h}`):

- OpenMP versions with `#pragma omp for`
- Functions: `parallel_multiply_ijk()`, `parallel_multiply_ikj()`, `parallel_multiply_jik()`, `parallel_multiply_jki()`, `parallel_multiply_kij()`, `parallel_multiply_kji()`
- All functions accept `thread_count` and `chunk` parameters
- All functions return execution time in seconds

Reference: `docs/omp_matrix_mult.c` shows example i-j-k and i-k-j implementations

### Tiled/Blocked Implementations

Location: `src/tiled/`

**Serial** (`serial/mm_blocked_serial.c`): Cache-blocked algorithm
**Parallel** (`parallel/mm_blocked_omp.{c,h}`): OpenMP parallel blocked version

Key considerations:

- Block size is a tunable parameter (affects cache performance)
- Must handle matrices where N is not divisible by block size
- Edge handling for partial blocks is critical

### Entry Points

**`src/main/main.c`**:

- Simple test driver for basic functionality
- Creates test matrices and runs basic validation

**`src/main/loop_permutations.c`**:

- Benchmarks all six loop permutations (serial and parallel)
- Calls `test_serial_loop_permutations()` and `test_parallel_loop_permutations()`
- Validates serial vs parallel results using i-j-k as reference

**`src/main/tiled.c`**:

- Benchmarks blocked implementations
- Tests different block sizes
- Validates against serial version

Entry points should:

- Accept matrix size N as argument (typical range: 1000-3000)
- Accept number of threads for OpenMP parallel regions
- Accept block size (tiled version only)
- Use `omp_get_wtime()` for timing
- Output results in format compatible with `benchmark/plot.py`

## Benchmarking

**Data flow:**

1. Run executables with varying parameters (N, threads, block size)
2. Output timing data to `benchmark/data/`
3. Generate plots with `benchmark/plot.py`
4. Plots saved to `benchmark/plots/`

**Expected output format**: CSV or space-separated values with columns for matrix size, threads, time, speedup, etc.

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
- Validation: Use epsilon comparison for floating-point results (`fabs(mat1[i][j] - mat2[i][j]) > EPSILON`)

**Matrix storage:**

- This codebase uses pointer-to-pointer (`double**`) via the `Matrix` typedef
- Accessed as `matrix[i][j]`
- Reference implementation (`docs/omp_matrix_mult.c`) uses static 2D arrays `double a[N][N]`

**Typical matrix sizes:**

- Small: 100-500 (fast testing)
- Medium: 1000-2000 (benchmarking)
- Large: 3000+ (performance evaluation)

## Documentation

- `docs/DenseMatrix.pdf`: Problem description and theoretical background
- `docs/omp_matrix_mult.c`: Reference implementation showing two versions (i-j-k and i-k-j) with both serial and parallel variants
