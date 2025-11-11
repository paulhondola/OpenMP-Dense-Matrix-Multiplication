#!/usr/bin/env python3

"""
Wrapper script that calls all individual plot scripts.
This maintains backward compatibility with existing Makefiles and workflows.
"""

import subprocess
import sys
from pathlib import Path


def main():
    script_dir = Path(__file__).parent
    plot_scripts = [
        "plot_serial_permutations.py",
        "plot_parallel_permutations.py",
        "plot_serial_parallel_scaling_classic.py",
        "plot_serial_parallel_scaling_improved.py",
        "plot_tiled.py",
    ]

    print("Generating plots...")
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
            result = subprocess.run(
                [sys.executable, str(script_path)],
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
