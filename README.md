# OpenMP: Matrix Multiplication

## Overview

This project provides multiple implementations of dense square matrix multiplication, in both serial and OpenMP-parallel forms. It includes classic (i, j, k) loop permutations and a blocked/tiled variant, along with simple validation and benchmarking helpers.

### (i, j, k) loop permutations

Implements all six permutations of (i, j, k) for square matrices:

- i-j-k, i-k-j, j-i-k, j-k-i, k-i-j, k-j-i

The i-j-k and i-k-j variants are common baselines; all permutations are included for comparison.

### Blocked matrix multiplication

Includes a blocked (tiled) algorithm in serial and with OpenMP to improve cache locality and performance on large matrices.

- Handles matrix sizes not divisible by the block size
- Block size can be tuned empirically for a given machine

### Dataset

- Square matrices N×N (typical N in the 1k–3k range for benchmarking)
- Values are generated at runtime at random

### Validation

Each version can be validated by comparing against the classical i-j-k implementation using the benchmarking and validation utilities in `src/benchmark/`.

## Building and running

The project uses a Makefile with the following commands:

- `make main` - Build and run the main entry point
- `make loop` - Build and run loop permutations benchmark
- `make tiled` - Build and run tiled/blocked benchmark

Recommended compiler: gcc-15 with OpenMP support (`-fopenmp`).

## Repository structure

```text
OpenMP-Dense-Matrix-Multiplication/
├── benchmark/
│   ├── data/                   # Input/output data for benchmarking
│   ├── plot.py                 # Plotting script
│   └── plots/                  # Generated plots
├── bin/                        # Output directory for built binaries
├── docs/
│   ├── DenseMatrix.pdf         # Problem description/background
│   └── omp_matrix_mult.c       # Lecture/example reference implementation
├── Makefile                    # Makefile for builds
├── src/
│   ├── benchmark/
│   │   ├── benchmark.c         # Benchmarking and validation utilities
│   │   └── benchmark.h
│   ├── common/
│   │   ├── matrix.c            # Matrix helpers (alloc/init/etc.)
│   │   └── matrix.h
│   ├── loop_permutations/
│   │   ├── parallel/
│   │   │   ├── mm_parallel.c   # OpenMP implementations for (i,j,k) permutations
│   │   │   └── mm_parallel.h
│   │   └── serial/
│   │       ├── mm_serial.c     # Serial implementations for (i,j,k) permutations
│   │       └── mm_serial.h
│   ├── main/
│   │   ├── main.c              # Simple test driver entry point
│   │   ├── loop_permutations.c # Entry point for loop-permutation experiments
│   │   └── tiled.c             # Entry point for blocked/tiled experiments
│   ├── tiled/
│   │   ├── parallel/
│   │   │   ├── mm_blocked_omp.c    # OpenMP blocked/tiled implementation
│   │   │   └── mm_blocked_omp.h
│   │   └── serial/
│   │       ├── mm_blocked_serial.c # Serial blocked/tiled implementation
│   │       └── mm_blocked_omp.h
│   └── utils/
│       ├── utils.c             # CSV file handling for benchmark data export
│       └── utils.h
└── README.md
```
