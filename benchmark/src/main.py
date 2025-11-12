#!/usr/bin/env python3

"""
Wrapper script that calls all individual plot scripts.
This maintains backward compatibility with existing Makefiles and workflows.
"""

import subprocess
import sys
import argparse
from pathlib import Path


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
    
    script_dir = Path(__file__).parent
    plot_scripts = [
        "plot_serial_permutations.py",
        "plot_parallel_permutations.py",
        "plot_serial_parallel_scaling_classic.py",
        "plot_serial_parallel_scaling_improved.py",
        "plot_tiled.py",
    ]

    print("Generating plots...")
    if folder_name:
        print(f"Using folder: {folder_name}")
    else:
        print("Using root data/ and plots/ directories")
    print("-" * 50)

    plots_created = 0
    plots_skipped = 0

    for script_name in plot_scripts:
        script_path = script_dir / script_name
        if not script_path.exists():
            print(f"Warning: Script not found: {script_path}", file=sys.stderr)
            plots_skipped += 1
            continue

        print(f"Running {script_name}...")
        try:
            cmd = [sys.executable, str(script_path)]
            if folder_name:
                cmd.append(folder_name)
            result = subprocess.run(
                cmd,
                cwd=str(script_dir),
                capture_output=False,
                text=True,
            )
            if result.returncode == 0:
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
