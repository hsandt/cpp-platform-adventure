#!/usr/bin/env python3

# Clean engine (engine-specific parts only, not third-party dependencies)

import sys
import shutil
import logging
from pathlib import Path


# Version check
if sys.version_info.minor < 6:
    print("Python version is %s, 3.6+ is required." % sys.version)
    sys.exit(1)


def clean_engine(engine_build_path: Path):
    """Delete engine build directory recursively"""
    if engine_build_path.exists():
        if engine_build_path.is_dir():
            shutil.rmtree(engine_build_path)
        else:
            logging.error(f"engine_build_path {engine_build_path} exists, but is not a dir")
            sys.exit(1)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    
    # get engine dir from this file's path 
    engine_path = Path(__file__).parent.absolute()
    engine_build_path = engine_path.joinpath("build")
    
    # clean engine
    logging.info(f"Cleaning engine build directory {engine_build_path}...")
    clean_engine(engine_build_path)
    logging.info("Done.")
