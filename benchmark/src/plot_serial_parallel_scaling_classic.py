#!/usr/bin/env python3

import matplotlib.pyplot as plt
from pathlib import Path
import sys

from utils import load_csv, aggregate_by_matrix_size, get_directories


def plot_serial_parallel_scaling_classic(folder_name: str = None, save: bool = True, show: bool = False) -> bool:
    data_dir, plots_dir = get_directories(Path(__file__), folder_name)

    df_classic = load_csv(data_dir, "serial_parallel_scaling_classic.csv")
    if df_classic is None:
        print("Skipping serial_parallel_scaling_classic plot - data file not available")
        return False

    # Aggregate by matrix size and chunk size
    agg_classic = aggregate_by_matrix_size(df_classic, ["MATRIX_SIZE", "CHUNK"])

    chunk_markers = ["o", "s", "^", "x", "D", "<", ">", "p", "h"]
    chunk_linestyles = ["-", "--", "-.", ":", "-.", "--", "-", ":", "-."]
    thread_colors = {2: "#e41a1c", 4: "#377eb8", 8: "#4daf4a"}

    fig1 = plt.figure(figsize=(10, 6))
    ax1 = fig1.add_subplot(111)
    chunk_sizes = sorted(df_classic["CHUNK"].unique())

    for chunk_idx, chunk in enumerate(chunk_sizes):
        chunk_data = agg_classic[agg_classic["CHUNK"] == chunk]
        x = chunk_data["MATRIX_SIZE"]

        # Plot serial baseline
        serial_mean = chunk_data[("SERIAL_BASELINE", "mean")]
        ax1.plot(
            x,
            serial_mean,
            label=f"Serial (chunk={chunk})",
            marker=chunk_markers[chunk_idx],
            linestyle=chunk_linestyles[chunk_idx],
            linewidth=2,
            markersize=6,
            color="#984ea3",
            alpha=0.7,
        )

        # Plot parallel versions
        for thread_count, color in thread_colors.items():
            col_name = f"P{thread_count}T"
            if (col_name, "mean") in chunk_data.columns:
                parallel_mean = chunk_data[(col_name, "mean")]
                ax1.plot(
                    x,
                    parallel_mean,
                    label=f"{thread_count}T (chunk={chunk})",
                    marker=chunk_markers[chunk_idx],
                    linestyle=chunk_linestyles[chunk_idx],
                    linewidth=2,
                    markersize=6,
                    color=color,
                    alpha=0.8,
                )

    ax1.set_xlabel("Matrix Size (N)", fontsize=12, fontweight="bold")
    ax1.set_ylabel("Speedup", fontsize=12, fontweight="bold")
    ax1.set_title(
        "Classic (i-j-k): Serial vs Parallel Scaling",
        fontsize=13,
        fontweight="bold",
    )
    ax1.legend(loc="best", framealpha=0.9, ncol=2, fontsize=8)
    ax1.grid(True, alpha=0.3, linestyle="--")
    plt.tight_layout()

    if save:
        output_path = plots_dir / "serial_parallel_scaling_classic.png"
        plt.savefig(output_path, dpi=300, bbox_inches="tight")
        print(f"Plot saved to {output_path}")

    if show:
        plt.show()
    else:
        plt.close()

    return True


def main():
    folder_name = sys.argv[1] if len(sys.argv) > 1 else None
    print("Creating serial parallel scaling classic plot...")
    try:
        plot_serial_parallel_scaling_classic(folder_name)
        print("Plot generation complete!")
    except Exception as e:
        print(
            f"Error creating serial parallel scaling classic plot: {e}", file=sys.stderr
        )
        sys.exit(1)


if __name__ == "__main__":
    main()
