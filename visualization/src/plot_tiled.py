#!/usr/bin/env python3

import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path
import sys

from utils import load_csv, get_directories, setup_plot_style


def plot_tiled(folder_name: str = None, save: bool = True, show: bool = False) -> bool:
    setup_plot_style()
    data_dir, plots_dir = get_directories(Path(__file__), folder_name)

    df = load_csv(data_dir, "tiled.csv")
    if df is None:
        print("Skipping tiled plot - data file not available")
        return False

    imp_cols = [
        "SERIAL_IKJ",
        "PARALLEL_IKJ",
        "SERIAL_TILED",
        "PARALLEL_TILED",
        "PARALLEL_TILED_TASKS",
    ]
    # Filter only existing columns
    imp_cols = [c for c in imp_cols if c in df.columns]

    # Melt dataframe
    melted_df = df.melt(
        id_vars=["MATRIX_SIZE", "BLOCK_SIZE"],
        value_vars=imp_cols,
        var_name="Implementation",
        value_name="Speedup",
    )

    # Rename implementations for better legend labels
    rename_map = {
        "SERIAL_IKJ": "Serial IKJ",
        "PARALLEL_IKJ": "Parallel IKJ",
        "SERIAL_TILED": "Serial Tiled",
        "PARALLEL_TILED": "Parallel Tiled",
        "PARALLEL_TILED_TASKS": "Parallel Tiled Tasks",
    }
    melted_df["Implementation"] = melted_df["Implementation"].replace(rename_map)

    ax = sns.lineplot(
        data=melted_df,
        x="MATRIX_SIZE",
        y="Speedup",
        hue="Implementation",
        style="BLOCK_SIZE",
        markers=True,
        dashes=False,
        errorbar="sd",
        linewidth=2,
        markersize=7,
        alpha=0.8,
    )

    ax.set_xlabel("Matrix Size (N)", fontsize=14, fontweight="bold")
    ax.set_ylabel("Speedup", fontsize=14, fontweight="bold")
    ax.set_title(
        "Tiled Matrix Multiplication: Speedup vs Matrix Size by Block Size",
        fontsize=16,
        fontweight="bold",
        pad=20,
    )
    sns.move_legend(
        ax, "upper left", bbox_to_anchor=(1.02, 1), borderaxespad=0, title="Legend"
    )

    # Use figure object for layout and saving
    fig = ax.get_figure()
    fig.tight_layout()

    if save:
        output_path = plots_dir / "tiled.png"
        fig.savefig(output_path, dpi=300, bbox_inches="tight")
        print(f"Plot saved to {output_path}")

    if show:
        plt.show()
    else:
        plt.close()

    return True


def main():
    folder_name = sys.argv[1] if len(sys.argv) > 1 else None
    print("Creating tiled plot...")
    try:
        plot_tiled(folder_name)
        print("Plot generation complete!")
    except Exception as e:
        print(f"Error creating tiled plot: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
