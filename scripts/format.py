#!/usr/bin/env python3
"""
format.py - Checks and formats C++ source files using clang-format.
"""

import argparse
import sys
import subprocess
import difflib
import shutil
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor, as_completed


# -------------------- Config -------------------- #
class Colors:
    RED = "\033[31m"
    GREEN = "\033[32m"
    CYAN = "\033[36m"
    YELLOW = "\033[33m"
    RESET = "\033[0m"


CLANG_BINARIES = ["clang-format-19", "clang-format"]
EXTENSIONS = {".cpp", ".h", ".hpp", ".cc", ".cxx"}
DIRECTORIES = ["include", "src", "tests"]
DIFF_CONTEXT_LINES = 0
MAX_WORKERS = 8
SEPARATOR = "===================="


def find_clang_format() -> str:
    """Find clang-format binary"""
    for binary in CLANG_BINARIES:
        path = shutil.which(binary)
        if path:
            return path
    sys.exit("Error: clang-format not found.")


def find_source_files(dirs) -> list[Path]:
    """Find all C++ source files in given directories"""
    files = []
    for directory in dirs:
        path = Path(directory)
        if not path.exists():
            continue
        for file_path in path.rglob("*"):
            if file_path.is_file() and file_path.suffix in EXTENSIONS:
                files.append(file_path)
    return sorted(files)


def format_file(clang_format, file_path, in_place=False):
    """Format a file, return colored diff and added/removed counts"""
    try:
        original = file_path.read_text(encoding="utf-8")

        result = subprocess.run(
            [clang_format, "-style=file", str(file_path)],
            capture_output=True,
            text=True,
        )
        if result.returncode != 0:
            print(f"Error formatting {file_path}: {result.stderr}")
            return None, 0, 0

        formatted = result.stdout
        if original == formatted:
            return None, 0, 0

        diff_lines = list(
            difflib.unified_diff(
                original.splitlines(keepends=True),
                formatted.splitlines(keepends=True),
                fromfile=str(file_path),
                tofile=str(file_path),
                n=DIFF_CONTEXT_LINES,
            )
        )

        added = 0
        removed = 0
        colored_diff = []
        for line in diff_lines:
            if line.startswith("+++") or line.startswith("---"):
                continue
            if line.startswith("+"):
                colored_diff.append(f"{Colors.GREEN}{line.rstrip()}{Colors.RESET}")
                added += 1
            elif line.startswith("-"):
                colored_diff.append(f"{Colors.RED}{line.rstrip()}{Colors.RESET}")
                removed += 1
            else:
                colored_diff.append(line.rstrip())

        if in_place:
            # Write directly to file
            file_path.write_text(formatted, encoding="utf-8")

        return "\n".join(colored_diff), added, removed

    except Exception as e:
        print(f"Error processing {file_path}: {e}")
        return None, 0, 0


# -------------------- Main -------------------- #
def main():
    parser = argparse.ArgumentParser(description="Checks and formats C++ source files using clang-format.")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "-c", "--check", action="store_true", help="Check for formatting violations without modifying files"
    )
    group.add_argument(
        "-f", "--format", action="store_true", help="Format files in-place"
    )
    args = parser.parse_args()

    clang_format = find_clang_format()
    files = find_source_files(DIRECTORIES)

    if not files:
        print("No files to format.")
        return

    violation_count = 0
    total_files_changed = 0
    total_added = 0
    total_removed = 0

    mode = "format" if args.format else "check"
    print(f"{'Formatting' if mode=='format' else 'Checking'} {len(files)} files...")

    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
        futures = {
            executor.submit(
                format_file, clang_format, f, in_place=(mode == "format")
            ): f
            for f in files
        }

        for future in as_completed(futures):
            file_path = futures[future]
            try:
                diff, added, removed = future.result()
                if diff:
                    print(SEPARATOR)
                    print(f"{Colors.CYAN}Changes in file: {file_path}{Colors.RESET}")
                    print(diff)
                    print(
                        f"\nSummary: {Colors.GREEN}+{added}{Colors.RESET} "
                        f"added, {Colors.RED}-{removed}{Colors.RESET} removed lines"
                    )
                    violation_count += 1
                    total_files_changed += 1
                    total_added += added
                    total_removed += removed
            except Exception as e:
                print(f"Error processing {file_path}: {e}")

    if violation_count > 0:
        print(SEPARATOR)
        if mode == "check":
            print(
                f"{Colors.RED}Formatting violations found in {total_files_changed} "
                f"file(s), total +{total_added}/-{total_removed} lines.{Colors.RESET}"
            )
            sys.exit(1)
        else:
            print(
                f"{Colors.GREEN}Formatting applied to {total_files_changed} "
                f"file(s), total +{total_added}/-{total_removed} lines.{Colors.RESET}"
            )
    else:
        print(f"{Colors.GREEN}All files are properly formatted.{Colors.RESET}")


if __name__ == "__main__":
    main()
