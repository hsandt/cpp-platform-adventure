#!/usr/bin/env python3

# Clean game-specific build folder (not engine)

import sys
import shutil
import logging
from pathlib import Path


# Version check
if sys.version_info.minor < 6:
    print("Python version is %s, 3.6+ is required." % sys.version)
    sys.exit(1)


def clean(build_path: Path):
    """Delete build directory recursively"""
    if build_path.exists():
        if build_path.is_dir():
            shutil.rmtree(build_path)
        else:
            logging.error(f"build_path {build_path} exists, but is not a dir")
            sys.exit(1)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    
    # get game project root dir from this file's path 
    root_path = Path(__file__).parent.absolute()
    build_path = root_path.joinpath("build")
    
    # clean game build folder
    logging.info(f"Cleaning game build directory {build_path}...")
    clean(build_path)
    logging.info("Done.")
