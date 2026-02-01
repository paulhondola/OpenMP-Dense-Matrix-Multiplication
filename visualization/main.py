#!/usr/bin/env python3

import sys
import argparse
from src.plot_serial_permutations import plot_serial_permutations
from src.plot_parallel_permutations import plot_parallel_permutations
from src.plot_serial_parallel_scaling_classic import (
    plot_serial_parallel_scaling_classic,
)
from src.plot_serial_parallel_scaling_improved import (
    plot_serial_parallel_scaling_improved,
)
from src.plot_tiled import plot_tiled


def main():
    parser = argparse.ArgumentParser(
        description="Generate plots from benchmark data",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        "folder",
        nargs="?",
        default=None,
        help="Subfolder name within benchmark/data/ and benchmark/plots/ (e.g., O0, O3). If not provided, uses root data/ and plots/ directories.",
    )

    args = parser.parse_args()
    folder_name = args.folder

    # Map script names to their plotting functions
    plot_functions = [
        ("plot_serial_permutations", plot_serial_permutations),
        ("plot_parallel_permutations", plot_parallel_permutations),
        ("plot_serial_parallel_scaling_classic", plot_serial_parallel_scaling_classic),
        (
            "plot_serial_parallel_scaling_improved",
            plot_serial_parallel_scaling_improved,
        ),
        ("plot_tiled", plot_tiled),
    ]

    print("Generating plots...")
    if folder_name:
        print(f"Using folder: {folder_name}")
    else:
        print("Using root data/ and plots/ directories")
    print("-" * 50)

    plots_created = 0
    plots_skipped = 0

    for script_name, plot_func in plot_functions:
        print(f"Running {script_name}...")
        try:
            success = plot_func(folder_name=folder_name, save=True, show=False)
            if success:
                plots_created += 1
            else:
                plots_skipped += 1
        except Exception as e:
            print(f"Error running {script_name}: {e}", file=sys.stderr)
            plots_skipped += 1

    print("-" * 50)
    print(f"Plots created: {plots_created}")
    if plots_skipped > 0:
        print(f"Plots skipped: {plots_skipped}")
    print("Plot generation complete!")


if __name__ == "__main__":
    main()
