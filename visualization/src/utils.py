#!/usr/bin/env python3

"""
Common utility functions for plot generation scripts.
"""

import pandas as pd
from pathlib import Path
from typing import List, Optional, Tuple
import sys
import seaborn as sns
import matplotlib.pyplot as plt


def setup_plot_style():
    """
    Configure seaborn theme for cleaner plots.
    """
    sns.set_theme(style="whitegrid", context="talk", palette="deep")
    plt.rcParams.update(
        {
            "figure.figsize": (12, 8),
            "axes.titlesize": 16,
            "axes.labelsize": 14,
            "xtick.labelsize": 12,
            "ytick.labelsize": 12,
            "legend.fontsize": 12,
            "lines.linewidth": 2.5,
        }
    )


def load_csv(data_dir: Path, filename: str) -> Optional[pd.DataFrame]:
    """
    Load CSV file with robust error handling.

    Args:
        data_dir: Full path to directory containing CSV files (already includes folder_name if applicable)
        filename: Name of the CSV file to load

    Returns:
        DataFrame if file exists and is valid, None otherwise
    """
    filepath = data_dir / filename

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


def get_directories(
    script_path: Path, folder_name: Optional[str] = None
) -> Tuple[Path, Path]:
    """
    Get the data and plots directories relative to the script location.
    Assumes scripts are in benchmark/src/ and data/plots are in benchmark/.

    Args:
        script_path: Path to the script file (typically Path(__file__))
        folder_name: Optional subfolder name (e.g., "O0", "O3") within data/ and plots/

    Returns:
        Tuple of (data_dir, plots_dir) Path objects
    """
    script_dir = script_path.parent
    # Go up two levels from src/ to visualization/ then to project root
    project_root = script_dir.parent.parent
    data_dir = project_root / "data" / "chrono"
    plots_dir = project_root / "data" / "plots"

    # If folder_name is provided, append it to both paths
    if folder_name:
        data_dir = data_dir / folder_name
        plots_dir = plots_dir / folder_name

    # Create directories if they don't exist
    data_dir.mkdir(parents=True, exist_ok=True)
    plots_dir.mkdir(parents=True, exist_ok=True)

    return data_dir, plots_dir
