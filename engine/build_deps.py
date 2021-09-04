#!/usr/bin/env python3

# Build and install third-party dependencies via dedicated scripts

import sys
import logging
from pathlib import Path
from subprocess import run, CalledProcessError
import multiprocessing

# Version check
if sys.version_info.minor < 6:
    print("Python version is %s, 3.6+ is required." % sys.version)
    sys.exit(1)


def build_deps(third_party_path: Path):
    """Build third-party dependencies of the engine by delegating to dedicated build scripts"""
    try:
        run(["python3", "build_fmt.py"], cwd=third_party_path, check=True)
    except CalledProcessError as e:
        logging.error(f"build_fmt.py failed")
        sys.exit(e.returncode)

    try:
        run(["python3", "build_sfml.py"], cwd=third_party_path, check=True)
    except CalledProcessError as e:
        logging.error(f"build_sfml.py failed")
        sys.exit(e.returncode)

    try:
        run(["python3", "build_yamlcpp.py"], cwd=third_party_path, check=True)
    except CalledProcessError as e:
        logging.error(f"build_yamlcpp.py failed")
        sys.exit(e.returncode)

    try:
        run(["python3", "build_rmlui.py"], cwd=third_party_path, check=True)
    except CalledProcessError as e:
        logging.error(f"build_rmlui.py failed")
        sys.exit(e.returncode)

if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    # get third-party dir from this file's path
    engine_path = Path(__file__).parent.absolute()
    third_party_path = engine_path.joinpath("third-party")

    # build deps
    logging.info(f"Building deps in {third_party_path}...")
    build_deps(third_party_path)
    logging.info("Done.")
