#!/usr/bin/env python3

import matplotlib.pyplot as plt
from pathlib import Path
import sys

from utils import load_csv, aggregate_by_matrix_size, get_directories, setup_plot_style


def plot_parallel_permutations(
    folder_name: str = None, save: bool = True, show: bool = False
) -> bool:
    setup_plot_style()
    data_dir, plots_dir = get_directories(Path(__file__), folder_name)

    permutation_names = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]
    permutation_colors = [
        "#e41a1c",
        "#377eb8",
        "#4daf4a",
        "#984ea3",
        "#ff7f00",
        "#ffff33",
    ]

    df = load_csv(data_dir, "parallel_permutations.csv")
    if df is None:
        print("Skipping parallel_permutations plot - data file not available")
        return False

    chunk_sizes = sorted(df["CHUNK"].unique())
    chunk_markers = ["o", "s", "^", "x", "D", "<", ">", "p", "h"]
    chunk_linestyles = ["-", "--", "-.", ":", "-.", "--", "-", ":", "-."]

    agg_df = aggregate_by_matrix_size(df, ["MATRIX_SIZE", "CHUNK"])

    plt.figure(figsize=(14, 8))

    for idx, perm in enumerate(permutation_names):
        col_name = perm
        base_color = permutation_colors[idx]

        for chunk_idx, chunk in enumerate(chunk_sizes):
            chunk_data = agg_df[agg_df["CHUNK"] == chunk]
            mean_col = (col_name, "mean")
            std_col = (col_name, "std")

            x = chunk_data["MATRIX_SIZE"]
            y = chunk_data[mean_col]
            yerr = chunk_data[std_col]

            label = f"{perm} (chunk={chunk})"
            plt.errorbar(
                x,
                y,
                yerr=yerr,
                label=label,
                color=base_color,
                marker=chunk_markers[chunk_idx],
                linestyle=chunk_linestyles[chunk_idx],
                capsize=4,
                linewidth=2,
                markersize=6,
                alpha=0.8,
            )

    plt.xlabel("Matrix Size (N)", fontsize=12, fontweight="bold")
    plt.ylabel("Speedup", fontsize=12, fontweight="bold")
    plt.title(
        f"Parallel Loop Permutations by Chunk Size",
        fontsize=14,
        fontweight="bold",
        pad=20,
    )
    plt.legend(loc="best", framealpha=0.9, ncol=3, fontsize=9)
    plt.tight_layout()

    if save:
        output_path = plots_dir / f"parallel_permutations.png"
        plt.savefig(output_path, dpi=300, bbox_inches="tight")
        print(f"Plot saved to {output_path}")

    if show:
        plt.show()
    else:
        plt.close()

    return True


def main():
    folder_name = sys.argv[1] if len(sys.argv) > 1 else None
    print("Creating parallel permutations plot...")
    try:
        plot_parallel_permutations(folder_name)
        print("Plot generation complete!")
    except Exception as e:
        print(f"Error creating parallel permutations plot: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
