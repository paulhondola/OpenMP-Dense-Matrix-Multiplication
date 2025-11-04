#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
from typing import List, Dict, Optional
import sys


class BenchmarkPlotter:

    def __init__(self, data_dir: str = "data", plots_dir: str = "plots"):
        script_dir = Path(__file__).parent
        self.data_dir = script_dir / data_dir
        self.plots_dir = script_dir / plots_dir

        # Create directories if they don't exist
        self.data_dir.mkdir(parents=True, exist_ok=True)
        self.plots_dir.mkdir(parents=True, exist_ok=True)

        self.permutation_names = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]
        self.permutation_colors = [
            "#e41a1c",
            "#377eb8",
            "#4daf4a",
            "#984ea3",
            "#ff7f00",
            "#ffff33",
        ]

    def load_csv(self, filename: str) -> Optional[pd.DataFrame]:
        """
        Load CSV file with robust error handling.

        Args:
            filename: Name of the CSV file to load

        Returns:
            DataFrame if file exists and is valid, None otherwise
        """
        filepath = self.data_dir / filename

        # Check if file exists
        if not filepath.exists():
            print(f"Warning: File not found: {filepath}", file=sys.stderr)
            return None

        # Check if file is readable
        if not filepath.is_file():
            print(f"Warning: Path is not a file: {filepath}", file=sys.stderr)
            return None

        # Check if file is empty
        if filepath.stat().st_size == 0:
            print(f"Warning: File is empty: {filepath}", file=sys.stderr)
            return None

        try:
            df = pd.read_csv(filepath)

            # Validate that DataFrame is not empty
            if df.empty:
                print(
                    f"Warning: CSV file is empty or has no data: {filepath}",
                    file=sys.stderr,
                )
                return None

            return df

        except pd.errors.EmptyDataError:
            print(
                f"Error: CSV file is empty or has no valid data: {filepath}",
                file=sys.stderr,
            )
            return None
        except pd.errors.ParserError as e:
            print(f"Error: Failed to parse CSV file {filepath}: {e}", file=sys.stderr)
            return None
        except PermissionError:
            print(f"Error: Permission denied when reading {filepath}", file=sys.stderr)
            return None
        except Exception as e:
            print(f"Error: Unexpected error reading {filepath}: {e}", file=sys.stderr)
            return None

    def aggregate_by_matrix_size(
        self, df: pd.DataFrame, groupby_cols: List[str]
    ) -> pd.DataFrame:
        return df.groupby(groupby_cols).agg(["mean", "std"]).reset_index()

    def plot_serial_permutations(self, save: bool = True, show: bool = False) -> bool:
        df = self.load_csv("serial_permutations.csv")
        if df is None:
            print("Skipping serial_permutations plot - data file not available")
            return False

        agg_df = self.aggregate_by_matrix_size(df, ["MATRIX_SIZE"])

        plt.figure(figsize=(12, 7))

        for idx, perm in enumerate(self.permutation_names):
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
                color=self.permutation_colors[idx],
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
            output_path = self.plots_dir / "serial_permutations.png"
            plt.savefig(output_path, dpi=300, bbox_inches="tight")
            print(f"Plot saved to {output_path}")

        if show:
            plt.show()
        else:
            plt.close()

        return True

    def plot_parallel_permutations(self, save: bool = True, show: bool = False) -> bool:
        df = self.load_csv("parallel_permutations.csv")
        if df is None:
            print("Skipping parallel_permutations plot - data file not available")
            return False

        chunk_sizes = sorted(df["CHUNK"].unique())
        chunk_markers = ["o", "s", "^", "x", "D", "<", ">", "p", "h"]
        chunk_linestyles = ["-", "--", "-.", ":", "-.", "--", "-", ":", "-."]

        agg_df = self.aggregate_by_matrix_size(df, ["MATRIX_SIZE", "CHUNK"])

        plt.figure(figsize=(14, 8))

        for idx, perm in enumerate(self.permutation_names):
            col_name = perm
            base_color = self.permutation_colors[idx]

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
        plt.grid(True, alpha=0.3, linestyle="--")
        plt.tight_layout()

        if save:
            output_path = self.plots_dir / f"parallel_permutations.png"
            plt.savefig(output_path, dpi=300, bbox_inches="tight")
            print(f"Plot saved to {output_path}")

        if show:
            plt.show()
        else:
            plt.close()

        return True

    def plot_classic_vs_improved(self, save: bool = True, show: bool = False) -> bool:
        df = self.load_csv("classic_vs_improved.csv")
        if df is None:
            print("Skipping classic_vs_improved plot - data file not available")
            return False

        # Aggregate by matrix size to average across different chunk sizes
        # For serial data, chunk size shouldn't matter, but we average for consistency
        agg_df = self.aggregate_by_matrix_size(df, ["MATRIX_SIZE"])

        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

        x = agg_df["MATRIX_SIZE"]
        baseline_mean = agg_df[("SERIAL_BASELINE", "mean")]
        improved_mean = agg_df[("IMPROVED_SERIAL", "mean")]

        ax1.plot(
            x,
            baseline_mean,
            label="Baseline (i-j-k)",
            marker="o",
            linewidth=2,
            markersize=8,
            color="#e41a1c",
        )
        ax1.plot(
            x,
            improved_mean,
            label="Improved (i-k-j)",
            marker="s",
            linewidth=2,
            markersize=8,
            color="#377eb8",
        )

        ax1.set_xlabel("Matrix Size (N)", fontsize=12, fontweight="bold")
        ax1.set_ylabel("Speedup", fontsize=12, fontweight="bold")
        ax1.set_title("Serial: Classic vs Improved", fontsize=13, fontweight="bold")
        ax1.legend(loc="best", framealpha=0.9)
        ax1.grid(True, alpha=0.3, linestyle="--")

        thread_counts = [2, 4, 8]
        colors_classic = ["#e41a1c", "#c41e1e", "#a42020"]
        colors_improved = ["#377eb8", "#2d6a9e", "#235684"]

        for idx, t in enumerate(thread_counts):
            classic_mean = agg_df[(f"P{t}T", "mean")]
            improved_mean = agg_df[(f"IP{t}T", "mean")]

            ax2.plot(
                x,
                classic_mean,
                label=f"Classic {t}T",
                marker="o",
                linewidth=2,
                markersize=7,
                color=colors_classic[idx],
                linestyle="-",
            )
            ax2.plot(
                x,
                improved_mean,
                label=f"Improved {t}T",
                marker="s",
                linewidth=2,
                markersize=7,
                color=colors_improved[idx],
                linestyle="--",
            )

        ax2.set_xlabel("Matrix Size (N)", fontsize=12, fontweight="bold")
        ax2.set_ylabel("Speedup", fontsize=12, fontweight="bold")
        ax2.set_title("Parallel: Classic vs Improved", fontsize=13, fontweight="bold")
        ax2.legend(loc="best", framealpha=0.9, ncol=2)
        ax2.grid(True, alpha=0.3, linestyle="--")

        plt.tight_layout()

        if save:
            output_path = self.plots_dir / "classic_vs_improved.png"
            plt.savefig(output_path, dpi=300, bbox_inches="tight")
            print(f"Plot saved to {output_path}")

        if show:
            plt.show()
        else:
            plt.close()

        return True

    def plot_tiled(self, save: bool = True, show: bool = False) -> bool:
        df = self.load_csv("tiled.csv")
        if df is None:
            print("Skipping tiled plot - data file not available")
            return False

        block_sizes = sorted(df["BLOCK_SIZE"].unique())
        block_markers = ["o", "s", "^", "x", "D", "<", ">", "p", "h"]
        block_linestyles = ["-", "--", "-.", ":", "-.", "--", "-", ":", "-."]

        # Aggregate by matrix size and block size
        agg_df = self.aggregate_by_matrix_size(df, ["MATRIX_SIZE", "BLOCK_SIZE"])

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
            output_path = self.plots_dir / "tiled.png"
            plt.savefig(output_path, dpi=300, bbox_inches="tight")
            print(f"Plot saved to {output_path}")

        if show:
            plt.show()
        else:
            plt.close()

        return True


def main():
    plotter = BenchmarkPlotter()

    print("Generating plots...")
    print("-" * 50)

    plots_created = 0
    plots_skipped = 0

    print("Creating serial permutations plot...")
    try:
        if plotter.plot_serial_permutations():
            plots_created += 1
        else:
            plots_skipped += 1
    except Exception as e:
        print(f"Error creating serial permutations plot: {e}", file=sys.stderr)
        plots_skipped += 1

    print("Creating parallel permutations plot...")
    try:
        if plotter.plot_parallel_permutations():
            plots_created += 1
        else:
            plots_skipped += 1
    except Exception as e:
        print(f"Error creating parallel permutations plot: {e}", file=sys.stderr)
        plots_skipped += 1

    print("Creating classic vs improved plot...")
    try:
        if plotter.plot_classic_vs_improved():
            plots_created += 1
        else:
            plots_skipped += 1
    except Exception as e:
        print(f"Error creating classic vs improved plot: {e}", file=sys.stderr)
        plots_skipped += 1

    print("Creating tiled plot...")
    try:
        if plotter.plot_tiled():
            plots_created += 1
        else:
            plots_skipped += 1
    except Exception as e:
        print(f"Error creating tiled plot: {e}", file=sys.stderr)
        plots_skipped += 1

    print("-" * 50)
    print(f"Plots created: {plots_created}")
    if plots_skipped > 0:
        print(f"Plots skipped: {plots_skipped}")
    print("Plot generation complete!")


if __name__ == "__main__":
    main()
