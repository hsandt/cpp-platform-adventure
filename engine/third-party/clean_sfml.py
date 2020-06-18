#!/usr/bin/env python3

# Clean SFML

import sys
import shutil
import logging
from pathlib import Path


# Version check
if sys.version_info.minor < 6:
    print("Python version is %s, 3.6+ is required." % sys.version)
    sys.exit(1)


def clean_sfml(sfml_build_path: Path):
    """Delete sfml build directory recursively"""
    if sfml_build_path.exists():
        if sfml_build_path.is_dir():
            shutil.rmtree(sfml_build_path)
        else:
            logging.error(f"sfml_build_path {sfml_build_path} exists, but is not a dir")
            sys.exit(1)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    
    # get third-party dir from this file's path 
    third_party_path = Path(__file__).parent.absolute()
    sfml_build_path = third_party_path.joinpath("build/SFML")
    
    # clean SFML in target path
    logging.info(f"Cleaning engine build directory {sfml_build_path}...")
    clean_sfml(sfml_build_path)
    logging.info("Done.")
