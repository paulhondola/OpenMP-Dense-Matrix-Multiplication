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

Each version can be validated by comparing against the classical i-j-k implementation inside the test directory.

## Building and running

- Recommended compiler: gcc-15 with OpenMP support (`-fopenmp`).
- See the repository structure below for entry points under `src/main/`.

## Repository structure

```text
OpenMP-Dense-Matrix-Multiplication/
  benchmark/
    data/                   # Input/output data for benchmarking
    plot.py                 # Plotting script
    plots/                  # Generated plots
  bin/                      # Suggested output directory for built binaries
  docs/
    DenseMatrix.pdf         # Problem description/background
    omp_matrix_mult.c       # Lecture/example reference implementation
  makefile                  # Makefile for builds
  src/
    common/
      matrix.c              # Matrix helpers (alloc/init/etc.)
      matrix.h
    loop_permutations/
      parallel/
        mm_parallel.c       # OpenMP implementations for (i,j,k) permutations
        mm_parallel.h
      serial/
        mm_serial.c         # Serial implementations for (i,j,k) permutations
        mm_serial.h
    main/
      loop_permutations.c   # Entry point for loop-permutation experiments
      tiled.c               # Entry point for blocked/tiled experiments
    test/
      validate.c            # Result validation utilities
      validate.h
    tiled/
      parallel/
        mm_blocked_omp.c    # OpenMP blocked/tiled implementation
        mm_blocked_omp.h
      serial/
        mm_blocked_serial.c # Serial blocked/tiled implementation
        mm_blocked_omp.h
```
