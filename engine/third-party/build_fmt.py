#!/usr/bin/env python3

# Build and install fmt

import sys
import logging
from pathlib import Path
from subprocess import run, CalledProcessError
import multiprocessing

# Version check
if sys.version_info.minor < 6:
    print("Python version is %s, 3.6+ is required." % sys.version)
    sys.exit(1)


def build_fmt(fmt_path: Path, fmt_build_path: Path, fmt_install_path: Path):
    """Build fmt from source path into build path"""
    # create directory with any intermediate parents, if needed
    # similar to Unix: mkdir -p
    Path(fmt_build_path).mkdir(parents=True, exist_ok=True)

    # We want:
    # - release build with CMAKE_BUILD_TYPE=Release (default)
    # - install locally in: engine/third-party/install/fmt/
    # - no doc with FMT_DOC:BOOL=OFF
    # - no test with FMT_TEST=OFF
    options = [
        "-G", "Unix Makefiles",
        f"-DCMAKE_INSTALL_PREFIX={fmt_install_path}",
        f"-DFMT_DOC:BOOL=OFF",
        f"-DFMT_TEST=OFF",
    ]
    try:
        run(["cmake", *options, fmt_path], cwd=fmt_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"cmake command failed")
        sys.exit(e.returncode)

    cpu_count = 1
    try:
        cpu_count = multiprocessing.cpu_count()
    except NotImplementedError as e:
        logging.error(f"multiprocessing.cpu_count() not implemented, defaulting to -j1")

    try:
        run(["make", f"-j{cpu_count}"], cwd=fmt_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"make command failed")
        sys.exit(e.returncode)

    try:
        run(["make", "install"], cwd=fmt_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"make install command failed")
        sys.exit(e.returncode)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    # get third-party dir from this file's path
    third_party_path = Path(__file__).parent.absolute()
    fmt_path = third_party_path.joinpath("fmt")
    fmt_build_path = third_party_path.joinpath("build/fmt")
    fmt_install_path = third_party_path.joinpath("install/fmt")

    # build fmt in target path
    logging.info(f"Building fmt from {fmt_path} in {fmt_build_path} and installing to {fmt_install_path}...")
    build_fmt(fmt_path, fmt_build_path, fmt_install_path)
    logging.info("Done.")
