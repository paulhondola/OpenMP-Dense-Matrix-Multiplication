#!/usr/bin/env python3

import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path
import sys

from utils import load_csv, get_directories, setup_plot_style


def plot_serial_parallel_scaling_improved(
    folder_name: str = None, save: bool = True, show: bool = False
) -> bool:
    setup_plot_style()
    data_dir, plots_dir = get_directories(Path(__file__), folder_name)

    df_improved = load_csv(data_dir, "serial_parallel_scaling_improved.csv")
    if df_improved is None:
        print(
            "Skipping serial_parallel_scaling_improved plot - data file not available"
        )
        return False

    # Identify implementation columns (SERIAL_BASELINE and P*T)
    # Note: The existing code used SERIAL_BASELINE, so we check for that.
    imp_cols = [
        c
        for c in df_improved.columns
        if c == "SERIAL_BASELINE" or (c.startswith("P") and c.endswith("T"))
    ]

    # Melt dataframe
    melted_df = df_improved.melt(
        id_vars=["MATRIX_SIZE", "CHUNK"],
        value_vars=imp_cols,
        var_name="Implementation",
        value_name="Speedup",
    )

    ax = sns.lineplot(
        data=melted_df,
        x="MATRIX_SIZE",
        y="Speedup",
        hue="Implementation",
        style="CHUNK",
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
        "Improved (i-k-j): Serial vs Parallel Scaling",
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
        output_path = plots_dir / "serial_parallel_scaling_improved.png"
        fig.savefig(output_path, dpi=300, bbox_inches="tight")
        print(f"Plot saved to {output_path}")

    if show:
        plt.show()
    else:
        plt.close()

    return True


def main():
    folder_name = sys.argv[1] if len(sys.argv) > 1 else None
    print("Creating serial parallel scaling improved plot...")
    try:
        plot_serial_parallel_scaling_improved(folder_name)
        print("Plot generation complete!")
    except Exception as e:
        print(
            f"Error creating serial parallel scaling improved plot: {e}",
            file=sys.stderr,
        )
        sys.exit(1)


if __name__ == "__main__":
    main()
