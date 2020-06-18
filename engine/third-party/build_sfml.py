#!/usr/bin/env python3

# Build SFML

import sys
import logging
from pathlib import Path
from subprocess import run, CalledProcessError


# Version check
if sys.version_info.minor < 6:
    print("Python version is %s, 3.6+ is required." % sys.version)
    sys.exit(1)


def build_sfml(sfml_path: Path, sfml_build_path: Path):
    """Build SFML from source path into build path"""
    # create directory with any intermediate parents, if needed
    # similar to Unix: mkdir -p
    Path(sfml_build_path).mkdir(parents=True, exist_ok=True)
    options = "-DSFML_BUILD_NETWORK=FALSE"
    try:
        run(["cmake", options, sfml_path], cwd=sfml_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"cmake command failed")
        sys.exit(e.returncode)
    try:
        run(["make"], cwd=sfml_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"make command failed")
        sys.exit(e.returncode)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    
    # get third-party dir from this file's path 
    third_party_path = Path(__file__).parent.absolute()
    sfml_path = third_party_path.joinpath("SFML")
    sfml_build_path = third_party_path.joinpath("build/SFML")
    
    # build SFML in target path
    logging.info(f"Building SFML from {sfml_path} in {sfml_build_path}...")
    build_sfml(sfml_path, sfml_build_path)
    logging.info("Done.")
