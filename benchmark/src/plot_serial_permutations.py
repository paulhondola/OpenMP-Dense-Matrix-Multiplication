#!/usr/bin/env python3

import matplotlib.pyplot as plt
from pathlib import Path
import sys

from utils import load_csv, aggregate_by_matrix_size, get_directories


def plot_serial_permutations(save: bool = True, show: bool = False) -> bool:
    data_dir, plots_dir = get_directories(Path(__file__))

    permutation_names = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]
    permutation_colors = [
        "#e41a1c",
        "#377eb8",
        "#4daf4a",
        "#984ea3",
        "#ff7f00",
        "#ffff33",
    ]

    df = load_csv(data_dir, "serial_permutations.csv")
    if df is None:
        print("Skipping serial_permutations plot - data file not available")
        return False

    agg_df = aggregate_by_matrix_size(df, ["MATRIX_SIZE"])

    plt.figure(figsize=(12, 7))

    for idx, perm in enumerate(permutation_names):
        col_name = perm
        mean_col = (col_name, "mean")
        std_col = (col_name, "std")

        x = agg_df["MATRIX_SIZE"]
        y = agg_df[mean_col]
        yerr = agg_df[std_col]

        plt.errorbar(
            x,
            y,
            yerr=yerr,
            label=perm,
            color=permutation_colors[idx],
            marker="o",
            linestyle="-",
            capsize=5,
            linewidth=2,
            markersize=6,
        )

    plt.xlabel("Matrix Size (N)", fontsize=12, fontweight="bold")
    plt.ylabel("Speedup", fontsize=12, fontweight="bold")
    plt.title(
        "Serial Loop Permutations: Speedup vs Matrix Size",
        fontsize=14,
        fontweight="bold",
        pad=20,
    )
    plt.legend(loc="best", framealpha=0.9)
    plt.grid(True, alpha=0.3, linestyle="--")
    plt.tight_layout()

    if save:
        output_path = plots_dir / "serial_permutations.png"
        plt.savefig(output_path, dpi=300, bbox_inches="tight")
        print(f"Plot saved to {output_path}")

    if show:
        plt.show()
    else:
        plt.close()

    return True


def main():
    print("Creating serial permutations plot...")
    try:
        plot_serial_permutations()
        print("Plot generation complete!")
    except Exception as e:
        print(f"Error creating serial permutations plot: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()

