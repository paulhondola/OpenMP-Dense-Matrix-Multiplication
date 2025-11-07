#!/usr/bin/env python3

"""
Common utility functions for plot generation scripts.
"""

import pandas as pd
from pathlib import Path
from typing import List, Optional, Tuple
import sys


def load_csv(data_dir: Path, filename: str) -> Optional[pd.DataFrame]:
    """
    Load CSV file with robust error handling.

    Args:
        data_dir: Directory containing CSV files
        filename: Name of the CSV file to load

    Returns:
        DataFrame if file exists and is valid, None otherwise
    """
    filepath = Path(__file__).parent.parent / data_dir / filename

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


def aggregate_by_matrix_size(df: pd.DataFrame, groupby_cols: List[str]) -> pd.DataFrame:
    """
    Aggregate DataFrame by matrix size and other specified columns.
    Computes mean and standard deviation for all numeric columns.

    Args:
        df: DataFrame to aggregate
        groupby_cols: List of column names to group by

    Returns:
        Aggregated DataFrame with mean and std columns
    """
    return df.groupby(groupby_cols).agg(["mean", "std"]).reset_index()


def get_directories(script_path: Path) -> Tuple[Path, Path]:
    """
    Get the data and plots directories relative to the script location.
    Assumes scripts are in benchmark/src/ and data/plots are in benchmark/.

    Args:
        script_path: Path to the script file (typically Path(__file__))

    Returns:
        Tuple of (data_dir, plots_dir) Path objects
    """
    script_dir = script_path.parent
    # Go up one level from src/ to benchmark/, then access data/ and plots/
    benchmark_dir = script_dir.parent
    data_dir = benchmark_dir / "data"
    plots_dir = benchmark_dir / "plots"

    # Create directories if they don't exist
    data_dir.mkdir(parents=True, exist_ok=True)
    plots_dir.mkdir(parents=True, exist_ok=True)

    return data_dir, plots_dir
