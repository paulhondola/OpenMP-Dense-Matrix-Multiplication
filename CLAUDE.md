# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Dense square matrix multiplication implementations in C using OpenMP. The project explores performance optimization through:

1. **Loop permutations**: All six permutations of (i,j,k) loops
2. **Blocked/tiled algorithms**: Cache-friendly tiled multiplication

## Build System

The project uses a Makefile (currently empty/to be implemented). Expected build commands:

```bash
# Build all implementations
make

# Build specific targets
make loop_permutations  # Entry point: src/main/loop_permutations.c
make tiled              # Entry point: src/main/tiled.c

# Clean build artifacts
make clean
```

**Compiler requirements:**

- gcc-15 with OpenMP support
- Must use `-fopenmp` flag
- Output binaries should go to `bin/` directory

## Architecture

### Core Components

**Matrix utilities** (`src/common/matrix.{c,h}`):

- Matrix allocation, initialization, deallocation
- Random value generation
- Helper functions used across all implementations

**Validation** (`src/test/validate.{c,h}`):

- Compares results against reference i-j-k implementation
- Uses epsilon comparison for floating-point values (see EPSILON in docs/omp_matrix_mult.c)

### Loop Permutation Implementations

Location: `src/loop_permutations/`

Six permutations for each variant (serial and parallel):

- **i-j-k** (classic): Row-major access for C, column-major for B
- **i-k-j**: Better cache locality for B
- **j-i-k**: Interchanges outer loops
- **j-k-i**: Different cache pattern
- **k-i-j**: K outer loop, updates entire C
- **k-j-i**: K outer loop variant

**Serial** (`serial/mm_serial.{c,h}`): Baseline implementations
**Parallel** (`parallel/mm_parallel.{c,h}`): OpenMP versions with `#pragma omp for`

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

**`src/main/loop_permutations.c`**:

- Benchmarks all six loop permutations
- Validates serial vs parallel results
- Outputs timing data for performance comparison

**`src/main/tiled.c`**:

- Benchmarks blocked implementations
- Tests different block sizes
- Validates against serial version

Both should:

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
- Validation: Use epsilon comparison for floating-point results (not exact equality)

**Matrix storage:**

- Use dynamically allocated 1D arrays accessed as 2D: `matrix[i*N + j]`
- OR use pointer-to-pointer for true 2D arrays
- Reference implementation uses static 2D arrays (less flexible for large N)

**Typical matrix sizes:**

- Small: 100-500 (fast testing)
- Medium: 1000-2000 (benchmarking)
- Large: 3000+ (performance evaluation)

## Documentation

- `docs/DenseMatrix.pdf`: Problem description and theoretical background
- `docs/omp_matrix_mult.c`: Reference implementation showing two versions (i-j-k and i-k-j) with both serial and parallel variants
