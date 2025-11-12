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

- Square matrices N×N (default sizes: 480, 640, 960, 1280, 1920)
- Values are generated at runtime with configurable seed (default: `time(NULL)` for unique runs)
- Random values range from -10 to 10 (configurable in `src/main/parameters.h`)

### Validation and Benchmarking

Each version is validated by comparing against reference implementations using the benchmarking and validation utilities in `src/benchmark/`:

- **Serial validation**: All serial permutations validated against serial i-j-k baseline
- **Parallel validation**: All parallel permutations validated against parallel i-j-k baseline
- **Serial vs Parallel scaling**: Compares serial baseline with parallel versions using 2, 4, and 8 threads for both i-j-k (classic) and i-k-j (improved) implementations
- **Tiled validation**: Validates blocked implementations against serial i-k-j baseline (tests serial i-k-j, parallel i-k-j, serial tiled, parallel tiled, and task-based parallel tiled)

Results are exported to CSV files in `benchmark/data/` (or `benchmark/data/{FOLDER}/` if a folder name is specified) and can be visualized using the plotting script.

## Building and Running

The project uses a Makefile with the following commands:

### Run Individual Benchmarks

**Note**: These targets run the executables. Build them first using `make build` or `make build_O3`.

All benchmark targets accept an optional `FOLDER` variable to specify the output directory:

- `make serial_loop FOLDER=O0` - Run serial loop permutations benchmark, save to `benchmark/data/O0/`
- `make parallel_loop FOLDER=O3` - Run parallel loop permutations benchmark, save to `benchmark/data/O3/`
- `make serial_parallel_scaling FOLDER=O0` - Run serial vs parallel scaling comparison, save to `benchmark/data/O0/`
- `make tiled FOLDER=O3` - Run tiled/blocked benchmark, save to `benchmark/data/O3/`

If `FOLDER` is not specified, files are written to `benchmark/data/` (root directory).

### Build Targets

- `make build` - Build all executables without optimization (`-Wall -Wextra -fopenmp`)
- `make build_O3` - Build all executables with optimization (`-O3 -march=native -Wall -Wextra -fopenmp`)
- `make all FOLDER=O0` - Build (without optimization) and run all benchmarks sequentially, save to `benchmark/data/O0/`
- `make all_O3 FOLDER=O3` - Build (with optimization) and run all benchmarks sequentially, save to `benchmark/data/O3/`

**Note**: The `FOLDER` variable is optional. If omitted, files are written to `benchmark/data/`.

### Plotting

- `make plot FOLDER=O0` - Generate all plots from benchmark data in `benchmark/data/O0/`
  - Requires Python 3 with pandas and matplotlib
  - Uses the wrapper script `benchmark/src/main.py` which calls individual plot scripts
  - Reads CSV files from `benchmark/data/{FOLDER}/` and saves plots to `benchmark/plots/{FOLDER}/`
  - Individual scripts can also be run directly: `python3 benchmark/src/main.py O0`
  - Plots are saved as PNG files (300 DPI)
  - If `FOLDER` is not specified, uses root `benchmark/data/` and `benchmark/plots/` directories

### Cleanup

- `make clear` - Clear benchmark data and plot files

**Recommended compiler**: gcc-15 with OpenMP support (`-fopenmp`).

**Compiler flags**:

- Basic build: `-Wall -Wextra -fopenmp`
- Optimized build: `-O3 -march=native -Wall -Wextra -fopenmp`

## Usage Examples

### Organizing Benchmarks by Optimization Level

A common workflow is to compare performance at different optimization levels:

```bash
# Build and run benchmarks without optimization, save to O0 folder
make build
make all FOLDER=O0

# Build and run benchmarks with optimization, save to O3 folder
make build_O3
make all_O3 FOLDER=O3

# Generate plots for both optimization levels
make plot FOLDER=O0
make plot FOLDER=O3
```

This creates separate directories:

- `benchmark/data/O0/` - Contains CSV files from unoptimized builds
- `benchmark/data/O3/` - Contains CSV files from optimized builds
- `benchmark/plots/O0/` - Contains plots for O0 data
- `benchmark/plots/O3/` - Contains plots for O3 data

### Running Individual Benchmarks

```bash
# Run a single benchmark with folder organization
make serial_loop FOLDER=O0
make parallel_loop FOLDER=O3

# Run without folder (saves to benchmark/data/ root)
make serial_loop
```

### Direct Program Execution

You can also run the executables directly with a folder argument:

```bash
./bin/serial_loop.exe O0
./bin/parallel_loop.exe O3
./bin/serial_parallel_scaling.exe O0
./bin/tiled.exe O0
```

## Repository Structure

```text
OpenMP-Dense-Matrix-Multiplication/
├── benchmark/
│   ├── data/                   # CSV benchmark data files
│   │   ├── O0/                 # Optimization level 0 data (optional subdirectory)
│   │   │   ├── serial_permutations.csv
│   │   │   ├── parallel_permutations.csv
│   │   │   ├── serial_parallel_scaling_classic.csv
│   │   │   ├── serial_parallel_scaling_improved.csv
│   │   │   └── tiled.csv
│   │   ├── O3/                 # Optimization level 3 data (optional subdirectory)
│   │   │   └── *.png
│   │   └── *.csv               # Root-level CSV files (if FOLDER not specified)
│   ├── plots/                  # Generated plot images (PNG)
│   │   ├── O0/                 # Optimization level 0 plots (optional subdirectory)
│   │   │   ├── serial_permutations.png
│   │   │   ├── parallel_permutations.png
│   │   │   ├── serial_parallel_scaling_classic.png
│   │   │   ├── serial_parallel_scaling_improved.png
│   │   │   └── tiled.png
│   │   ├── O3/                 # Optimization level 3 plots (optional subdirectory)
│   │   │   └── *.png
│   │   └── *.png               # Root-level plots (if FOLDER not specified)
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
- Serial and parallel variants (including task-based parallel implementation)
- Tunable block size parameter
- Uses i-k-j loop ordering for better cache locality

### Configuration (`src/main/parameters.h`)

Centralized configuration for all benchmarks:

- **Matrix sizes**: Default `{480, 640, 960, 1280, 1920}` (configurable via `MATRIX_SIZES`)
- **Thread count**: Default `10` (configurable via `THREAD_COUNT`)
- **Chunk sizes**: Default `{48, 96, 192}` for parallel scheduling (configurable via `CHUNK_SIZES`)
- **Block sizes**: Default `{48, 96, 128}` for tiled multiplication (configurable via `BLOCK_SIZES`)
- **Random seed**: Default `time(NULL)` for unique runs per execution (configurable via `SEED`)
- **Validation tolerance**: Default `1e-6` for floating-point comparison (configurable via `EPSILON`)
- **Debug flags**: `DEBUG` (enabled by default) and `DEBUG_MATRIX` (commented out) for verbose output

### Utilities (`src/utils/`)

CSV file handling and output directory management:

- **CSV file operations**: Functions to write benchmark results to CSV files with configurable output directory
- **`set_output_folder()`**: Sets the output folder name (e.g., "O0", "O3") for organizing benchmark data by optimization level or other criteria
- **`ensure_directory_exists()`**: Creates output directories recursively if they don't exist
- **`open_csv_file()`**: Opens CSV files in `benchmark/data/{folder_name}/` or `benchmark/data/` if no folder specified
- **Output files**: `serial_permutations.csv`, `parallel_permutations.csv`, `serial_parallel_scaling_classic.csv`, `serial_parallel_scaling_improved.csv`, `tiled.csv`
- **Command line integration**: All C programs (`serial_loop`, `parallel_loop`, `serial-parallel-scaling`, `tiled`) accept an optional folder name as a command line argument to organize output files

### Plotting Scripts (`benchmark/src/`)

Modular plotting system with individual scripts for each plot type:

- **Modular design**: Five separate Python scripts, one per plot type:
  - `plot_serial_permutations.py` - Serial loop permutations: Speedup vs matrix size for all 6 loop orderings (i-j-k baseline)
  - `plot_parallel_permutations.py` - Parallel loop permutations: Speedup vs matrix size with separate lines per chunk size (parallel i-j-k baseline)
  - `plot_serial_parallel_scaling_classic.py` - Classic (i-j-k): Speedup vs matrix size comparing serial baseline with parallel versions using 2, 4, 8 threads
  - `plot_serial_parallel_scaling_improved.py` - Improved (i-k-j): Speedup vs matrix size comparing serial baseline with parallel versions using 2, 4, 8 threads
  - `plot_tiled.py` - Tiled implementations: Speedup vs matrix size with separate lines per block size (serial i-k-j baseline)
- **Common utilities** (`utils.py`): Shared functions for CSV loading, data aggregation, and directory management
  - `get_directories()`: Returns data and plots directories, optionally with folder name subdirectory
  - `load_csv()`: Loads CSV files from the specified data directory (with optional folder name)
- **Wrapper script** (`main.py`): Convenience script that runs all individual plot scripts sequentially
  - Accepts optional folder name argument: `python3 benchmark/src/main.py O0`
  - Passes folder name to all individual plot scripts
- **Features**:
  - Automatically aggregates duplicate measurements (averages when multiple runs per matrix size exist)
  - Uses distinct markers and line styles for parameter variations (chunk sizes, block sizes)
  - Plots saved as high-resolution PNG files in `benchmark/plots/{folder_name}/` or `benchmark/plots/`
  - Supports organizing data and plots by optimization level (O0, O3) or other criteria via folder names
