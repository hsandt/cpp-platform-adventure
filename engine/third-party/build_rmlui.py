#!/usr/bin/env python3

# Build and install RmlUi

import sys
import logging
from pathlib import Path
from subprocess import run, CalledProcessError
import multiprocessing

# Version check
if sys.version_info.minor < 6:
    print("Python version is %s, 3.6+ is required." % sys.version)
    sys.exit(1)


def build_rmlui(rmlui_path: Path, rmlui_build_path: Path, rmlui_install_path: Path):
    """Build RmlUi from source path into build path"""
    # create directory with any intermediate parents, if needed
    # similar to Unix: mkdir -p
    Path(rmlui_build_path).mkdir(parents=True, exist_ok=True)

    # We want:
    # - dynamic lib with BUILD_SHARED_LIBS=ON (default)
    # - release build with CMAKE_BUILD_TYPE=Release (default)
    # - install locally in: engine/third-party/install/RmlUi/
    options = [
        "-G", "Unix Makefiles",
        f"-DCMAKE_INSTALL_PREFIX={rmlui_install_path}",
    ]
    try:
        run(["cmake", *options, rmlui_path], cwd=rmlui_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"cmake command failed")
        sys.exit(e.returncode)

    cpu_count = 1
    try:
        cpu_count = multiprocessing.cpu_count()
    except NotImplementedError as e:
        logging.error(f"multiprocessing.cpu_count() not implemented, defaulting to -j1")

    try:
        run(["make", f"-j{cpu_count}"], cwd=rmlui_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"make command failed")
        sys.exit(e.returncode)

    try:
        run(["make", "install"], cwd=rmlui_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"make install command failed")
        sys.exit(e.returncode)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    # get third-party dir from this file's path
    third_party_path = Path(__file__).parent.absolute()
    rmlui_path = third_party_path.joinpath("RmlUi")
    rmlui_build_path = third_party_path.joinpath("build/RmlUi")
    rmlui_install_path = third_party_path.joinpath("install/RmlUi")

    # build RmlUi in target path
    logging.info(f"Building RmlUi from {rmlui_path} in {rmlui_build_path} and installing to {rmlui_install_path}...")
    build_rmlui(rmlui_path, rmlui_build_path, rmlui_install_path)
    logging.info("Done.")
