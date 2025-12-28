#!/usr/bin/env python3

import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path
import sys

from utils import load_csv, get_directories, setup_plot_style


def plot_serial_permutations(
    folder_name: str = None, save: bool = True, show: bool = False
) -> bool:
    setup_plot_style()
    data_dir, plots_dir = get_directories(Path(__file__), folder_name)

    df = load_csv(data_dir, "serial_permutations.csv")
    if df is None:
        print("Skipping serial_permutations plot - data file not available")
        return False

    permutation_names = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]

    # Melt the dataframe for seaborn
    melted_df = df.melt(
        id_vars=["MATRIX_SIZE"],
        value_vars=permutation_names,
        var_name="Permutation",
        value_name="Speedup",
    )

    plt.figure(figsize=(12, 8))

    sns.lineplot(
        data=melted_df,
        x="MATRIX_SIZE",
        y="Speedup",
        hue="Permutation",
        style="Permutation",
        markers=True,
        dashes=False,
        errorbar="sd",
        linewidth=2.5,
        markersize=8,
    )

    plt.xlabel("Matrix Size (N)", fontsize=14, fontweight="bold")
    plt.ylabel("Speedup", fontsize=14, fontweight="bold")
    plt.title(
        "Serial Loop Permutations: Speedup vs Matrix Size",
        fontsize=16,
        fontweight="bold",
        pad=20,
    )
    plt.legend(title="Permutation", loc="best", framealpha=0.9)
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
    folder_name = sys.argv[1] if len(sys.argv) > 1 else None
    print("Creating serial permutations plot...")
    try:
        plot_serial_permutations(folder_name)
        print("Plot generation complete!")
    except Exception as e:
        print(f"Error creating serial permutations plot: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
