#!/usr/bin/env python3

import matplotlib.pyplot as plt
from pathlib import Path
import sys

from utils import load_csv, aggregate_by_matrix_size, get_directories


def plot_tiled(save: bool = True, show: bool = False) -> bool:
    data_dir, plots_dir = get_directories(Path(__file__))

    df = load_csv(data_dir, "tiled.csv")
    if df is None:
        print("Skipping tiled plot - data file not available")
        return False

    block_sizes = sorted(df["BLOCK_SIZE"].unique())
    block_markers = ["o", "s", "^", "x", "D", "<", ">", "p", "h"]
    block_linestyles = ["-", "--", "-.", ":", "-.", "--", "-", ":", "-."]

    # Aggregate by matrix size and block size
    agg_df = aggregate_by_matrix_size(df, ["MATRIX_SIZE", "BLOCK_SIZE"])

    plt.figure(figsize=(14, 8))

    # Define colors and base labels for each implementation
    implementations = [
        ("SERIAL_IKJ", "Serial IKJ", "#1f77b4"),
        ("PARALLEL_IKJ", "Parallel IKJ", "#ff7f0e"),
        ("SERIAL_TILED", "Serial Tiled", "#2ca02c"),
        ("PARALLEL_TILED", "Parallel Tiled", "#d62728"),
    ]

    for col_name, base_label, base_color in implementations:
        for block_idx, block_size in enumerate(block_sizes):
            block_data = agg_df[agg_df["BLOCK_SIZE"] == block_size]
            mean_col = (col_name, "mean")
            std_col = (col_name, "std")

            if mean_col not in block_data.columns:
                continue

            x = block_data["MATRIX_SIZE"]
            y = block_data[mean_col]
            yerr = block_data[std_col]

            label = f"{base_label} (block={block_size})"
            plt.errorbar(
                x,
                y,
                yerr=yerr,
                label=label,
                color=base_color,
                marker=block_markers[block_idx % len(block_markers)],
                linestyle=block_linestyles[block_idx % len(block_linestyles)],
                capsize=4,
                linewidth=2,
                markersize=6,
                alpha=0.8,
            )

    plt.xlabel("Matrix Size (N)", fontsize=12, fontweight="bold")
    plt.ylabel("Speedup", fontsize=12, fontweight="bold")
    plt.title(
        "Tiled Matrix Multiplication: Speedup vs Matrix Size by Block Size",
        fontsize=14,
        fontweight="bold",
        pad=20,
    )
    plt.legend(loc="best", framealpha=0.9, ncol=3, fontsize=9)
    plt.grid(True, alpha=0.3, linestyle="--")
    plt.tight_layout()

    if save:
        output_path = plots_dir / "tiled.png"
        plt.savefig(output_path, dpi=300, bbox_inches="tight")
        print(f"Plot saved to {output_path}")

    if show:
        plt.show()
    else:
        plt.close()

    return True


def main():
    print("Creating tiled plot...")
    try:
        plot_tiled()
        print("Plot generation complete!")
    except Exception as e:
        print(f"Error creating tiled plot: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
