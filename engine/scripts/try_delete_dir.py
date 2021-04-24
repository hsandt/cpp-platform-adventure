#!/usr/bin/env python3

# Delete a full directory if it exists, empty or not

# Usage:
# delete_dir.py dir_path
#
# dir_path     path to directory to delete

import os
import sys
import argparse
import shutil
import logging
from pathlib import Path


# Version check
if sys.version_info.minor < 6:
    print("Python version is %s, 3.6+ is required." % sys.version)
    sys.exit(1)


def try_delete_dir_content(dir_path: str) -> bool:
    """
    If directory exists at `dir_path`, delete it recursively and return True.
    Else, return False.

    """
    path = Path(dir_path)
    if path.exists():
        if path.is_dir():
            for root, dirs, files in os.walk(dir_path):
                for f in files:
                    os.remove(os.path.join(root, f))
                for d in dirs:
                    shutil.rmtree(os.path.join(root, d))
            return True
        else:
            logging.error(f"path {dir_path} exists, but is not a dir")
            sys.exit(1)

    return False

def try_delete_dir_contents(dir_paths: [str]) -> bool:
    """
    Call try_delete_dir_content for every directory path in dir_paths
    Return true iff all directories were found

    """
    total_success = True

    for dir_path in dir_paths:
        success = try_delete_dir_content(dir_path)
        total_success = total_success and success

    return total_success

if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    parser = argparse.ArgumentParser(description='Delete any directory content recursively, preserving the directory itself.')
    parser.add_argument('dir_paths', type=str, nargs='+', help='path of directory to delete')
    args = parser.parse_args()

    logging.info(f"Trying to delete directory contents for: {args.dir_paths}...")
    deleted_all_dir_contents = try_delete_dir_contents(args.dir_paths)
    if deleted_all_dir_contents:
        logging.info("Deleted all directory contents.")
    else:
        logging.info("Some directories were not found and could not be deleted.")
