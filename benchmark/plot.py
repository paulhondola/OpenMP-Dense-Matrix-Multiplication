#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
from typing import List, Dict, Optional


class BenchmarkPlotter:

    def __init__(self, data_dir: str = "data", plots_dir: str = "plots"):
        script_dir = Path(__file__).parent
        self.data_dir = script_dir / data_dir
        self.plots_dir = script_dir / plots_dir
        self.plots_dir.mkdir(exist_ok=True)

        self.permutation_names = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]
        self.permutation_colors = [
            "#e41a1c",
            "#377eb8",
            "#4daf4a",
            "#984ea3",
            "#ff7f00",
            "#ffff33",
        ]

    def load_csv(self, filename: str) -> pd.DataFrame:
        filepath = self.data_dir / filename
        return pd.read_csv(filepath)

    def aggregate_by_matrix_size(
        self, df: pd.DataFrame, groupby_cols: List[str]
    ) -> pd.DataFrame:
        return df.groupby(groupby_cols).agg(["mean", "std"]).reset_index()

    def plot_serial_permutations(self, save: bool = True, show: bool = False):
        df = self.load_csv("serial_permutations.csv")

        agg_df = self.aggregate_by_matrix_size(df, ["MATRIX_SIZE"])

        plt.figure(figsize=(12, 7))

        for idx, perm in enumerate(self.permutation_names):
            col_name = f"{perm}_SPEEDUP"
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

    def plot_parallel_permutations(
        self,
        threads: Optional[int] = None,
        chunk: Optional[int] = None,
        save: bool = True,
        show: bool = False,
    ):
        df = self.load_csv("parallel_permutations.csv")

        title_parts = []
        if threads is not None:
            df = df[df["THREADS"] == threads]
            title_parts.append(f"{threads} Threads")

        title_suffix = (
            f" ({', '.join(title_parts)})" if title_parts else " (All Configurations)"
        )

        groupby_cols = ["MATRIX_SIZE"]
        if threads is None:
            groupby_cols.append("THREADS")

        agg_df = self.aggregate_by_matrix_size(df, groupby_cols)

        plt.figure(figsize=(12, 7))

        for idx, perm in enumerate(self.permutation_names):
            col_name = f"{perm}_SPEEDUP"
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
            f"Parallel Loop Permutations: Speedup vs Matrix Size{title_suffix}",
            fontsize=14,
            fontweight="bold",
            pad=20,
        )
        plt.legend(loc="best", framealpha=0.9)
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

    def plot_parallel_by_chunk(self, save: bool = True, show: bool = False):
        df = self.load_csv("parallel_permutations.csv")

        chunk_sizes = sorted(df["CHUNK"].unique())
        chunk_markers = ["o", "s", "^"]
        chunk_linestyles = ["-", "--", "-."]

        agg_df = self.aggregate_by_matrix_size(df, ["MATRIX_SIZE", "CHUNK"])

        plt.figure(figsize=(14, 8))

        for idx, perm in enumerate(self.permutation_names):
            col_name = f"{perm}_SPEEDUP"
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
            output_path = self.plots_dir / f"parallel_by_chunk.png"
            plt.savefig(output_path, dpi=300, bbox_inches="tight")
            print(f"Plot saved to {output_path}")

        if show:
            plt.show()
        else:
            plt.close()

    def plot_classic_vs_improved(self, save: bool = True, show: bool = False):
        df = self.load_csv("classic_vs_improved.csv")

        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

        x = df["MATRIX_SIZE"]
        ax1.plot(
            x,
            df["SERIAL_BASELINE"],
            df["IMPROVED_SERIAL_SPEEDUP"],
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
            ax2.plot(
                x,
                df[f"P{t}T_SPEEDUP"],
                label=f"Classic {t}T",
                marker="o",
                linewidth=2,
                markersize=7,
                color=colors_classic[idx],
                linestyle="-",
            )
            ax2.plot(
                x,
                df[f"IP{t}T_SPEEDUP"],
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


def main():
    plotter = BenchmarkPlotter()

    print("Generating plots...")
    print("-" * 50)

    print("Creating serial permutations plot...")
    plotter.plot_serial_permutations()

    print("Creating parallel permutations plot...")
    plotter.plot_parallel_permutations()

    print("Creating parallel by chunk plot...")
    plotter.plot_parallel_by_chunk()

    print("Creating classic vs improved plot...")
    plotter.plot_classic_vs_improved()

    print("-" * 50)
    print("All plots generated successfully!")


if __name__ == "__main__":
    main()
