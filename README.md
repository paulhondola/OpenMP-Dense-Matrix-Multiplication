# OpenMP: Matrix Multiplication

## Overview

This project provides multiple implementations of dense square matrix multiplication, in both serial and OpenMP-parallel forms. It includes classic (i, j, k) loop permutations and a blocked/tiled variant, along with comprehensive validation and benchmarking tools with automated plotting.

### (i, j, k) loop permutations

Implements all six permutations of (i, j, k) for square matrices (take C = A * B):

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

Results are exported to CSV files in `data/chrono/` (or `data/chrono/{FOLDER}/` if a folder name is specified) and can be visualized using the plotting script.

## Building and Running

The project uses **Meson** for building and managing the project.

### Prerequisites

- **Meson**
- **Ninja** (usually installed with Meson)
- **GCC/Clang** (with OpenMP support)
- **Python 3** (for visualization, with `pandas`, `seaborn`, `matplotlib`)

### Build Steps

1.  **Configure the build**:
    ```bash
    cd benchmark
    meson setup build
    ```
    For an optimized build (recommended for benchmarking):
    ```bash
    meson setup build -Doptimization=3 -Ddebug=false
    ```

2.  **Compile**:
    ```bash
    meson compile -C build
    ```

### Run Benchmarks

You can run benchmarks using Meson's `run_target` wrappers. These wrappers handle executing the binaries.

- **Serial Loop**:
  ```bash
  meson compile -C build run_serial
  ```
- **Parallel Loop**:
  ```bash
  meson compile -C build run_parallel
  ```
- **Scaling Comparison**:
  ```bash
  meson compile -C build run_scaling
  ```
- **Tiled Multiplication**:
  ```bash
  meson compile -C build run_tiled
  ```

By default, these Meson run targets execute the benchmarks with the `O3` folder argument, saving data to `data/chrono/O3/`.

**Direct Execution**:
The compiled executables are located in `benchmark/build/`.
```bash
# Save results to data/chrono/O3/
./benchmark/build/serial_loop O3
./benchmark/build/parallel_loop O3
```

### Plotting

The visualization scripts are located in the `visualization/` directory.

-   **Generate all plots**:
    ```bash
    cd visualization
    uv run main.py
    ```
-   **Generate plots for a specific folder** (e.g., `O3`):
    ```bash
    cd visualization
    uv run main.py O3
    ```

Plots are saved to `data/plots/` (or `data/plots/O3/`).

### Cleanup

-   **Clean build directory**:
    ```bash
    cd benchmark
    meson compile -C build --clean
    ```

## Usage Examples

### Workflow: Compare Optimization Levels

1.  **Build and run unoptimized (O0)**:
    ```bash
    cd benchmark
    meson setup build_O0 -Doptimization=0
    meson compile -C build_O0
    
    ../benchmark/build_O0/serial_loop O0
    ../benchmark/build_O0/parallel_loop O0
    ../benchmark/build_O0/serial_parallel_scaling O0
    ../benchmark/build_O0/tiled O0
    cd ..
    ```

2.  **Build and run optimized (O3)**:
    ```bash
    cd benchmark
    meson setup build_O3 -Doptimization=3
    meson compile -C build_O3

    ../benchmark/build_O3/serial_loop O3
    ../benchmark/build_O3/parallel_loop O3
    # ... etc
    cd ..
    ```

3.  **Generate Plots**:
    ```bash
    cd visualization
    uv run main.py O0
    uv run main.py O3
    ```

## Repository Structure

```text
OpenMP-Dense-Matrix-Multiplication/
├── benchmark/
│   ├── meson.build             # Meson build definition
│   ├── src/
│   │   ├── benchmark/          # Benchmarking & validation logic
│   │   ├── loop_permutations/  # Serial & Parallel loop implementations
│   │   ├── main/               # Executable entry points
│   │   ├── matrix/             # Matrix utility functions
│   │   ├── tiled/              # Tiled implementations
│   │   └── utils/              # C utilities (file I/O)
│   └── docs/                   # Documentation resources
├── visualization/
│   ├── main.py                 # Main plot generation script
│   └── src/                    # Individual plotting scripts & utils
│       ├── plot_*.py
│       └── utils.py
├── data/
│   ├── chrono/                 # Benchmark CSV output
│   └── plots/                  # Generated PNG plots
├── README.md
└── LICENSE
```

## Key Components

### Matrix Utilities (`benchmark/src/matrix/`)

- Matrix allocation, initialization, and deallocation
- Random value generation with configurable seed
- Validation with epsilon comparison for floating-point values
- Helper functions used across all implementations

### Benchmarking (`benchmark/src/benchmark/`)

- Validation functions comparing results against reference implementations
- Timing measurements using `omp_get_wtime()`
- Speedup calculations
- Test functions for serial/parallel permutations, classic vs improved, and tiled algorithms

### Loop Permutations (`benchmark/src/loop_permutations/`)

- **Serial**: Baseline implementations without parallelization
- **Parallel**: OpenMP versions with configurable thread count and chunk size
- All six loop orderings (i-j-k, i-k-j, j-i-k, j-k-i, k-i-j, k-j-i)
- Function pointer arrays for dynamic selection

### Tiled Implementations (`benchmark/src/tiled/`)

- Cache-blocked algorithms for improved performance
- Serial and parallel variants (including task-based parallel implementation)
- Tunable block size parameter
- Uses i-k-j loop ordering for better cache locality

### Configuration (`benchmark/src/main/parameters.h`)

Centralized configuration for all benchmarks:

- **Matrix sizes**: Default `{480, 640, 960, 1280, 1920}` (configurable via `MATRIX_SIZES`)
- **Thread count**: Default `10` (configurable via `THREAD_COUNT`)
- **Chunk sizes**: Default `{48, 96, 192}` for parallel scheduling (configurable via `CHUNK_SIZES`)
- **Block sizes**: Default `{48, 96, 128}` for tiled multiplication (configurable via `BLOCK_SIZES`)
- **Random seed**: Default `time(NULL)` for unique runs per execution (configurable via `SEED`)
- **Validation tolerance**: Default `1e-6` for floating-point comparison (configurable via `EPSILON`)
- **Debug flags**: `DEBUG` (enabled by default) and `DEBUG_MATRIX` (commented out) for verbose output

### Utilities (`benchmark/src/utils/`)

CSV file handling and output directory management:

- **CSV file operations**: Functions to write benchmark results to CSV files with configurable output directory
- **`set_output_folder()`**: Sets the output folder name (e.g., "O0", "O3") for organizing benchmark data by optimization level or other criteria
- **`ensure_directory_exists()`**: Creates output directories recursively if they don't exist
- **`open_csv_file()`**: Opens CSV files in `data/chrono/{folder_name}/` or `data/chrono/` if no folder specified
- **Output files**: `serial_permutations.csv`, `parallel_permutations.csv`, `serial_parallel_scaling_classic.csv`, `serial_parallel_scaling_improved.csv`, `tiled.csv`
- **Command line integration**: All executables accept an optional folder name as a command line argument to organize output files

### Plotting Scripts (`visualization/src/`)

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
- **Wrapper script** (`visualization/main.py`): Convenience script that runs all individual plot scripts sequentially
  - Run via uv: `cd visualization && uv run main.py O0`
  - Passes folder name to all individual plot scripts
- **Features**:
  - Automatically aggregates duplicate measurements (averages when multiple runs per matrix size exist)
  - Uses distinct markers and line styles for parameter variations (chunk sizes, block sizes)
  - Plots saved as high-resolution PNG files in `data/plots/{folder_name}/` or `data/plots/`
  - Supports organizing data and plots by optimization level (O0, O3) or other criteria via folder names
