#!/usr/bin/env python3

# Build and install SFML

import sys
import logging
from pathlib import Path
from subprocess import run, CalledProcessError
import multiprocessing

# Version check
if sys.version_info.minor < 6:
    print("Python version is %s, 3.6+ is required." % sys.version)
    sys.exit(1)


def build_sfml(sfml_path: Path, sfml_build_path: Path, sfml_install_path: Path):
    """Build SFML from source path into build path"""
    # create directory with any intermediate parents, if needed
    # similar to Unix: mkdir -p
    Path(sfml_build_path).mkdir(parents=True, exist_ok=True)

    # See https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php for compile options
    # We want:
    # - static libs for static linking (bigger, self-contained game executable)
    # - all modules except NETWORK
    # - no package config
    # - install locally in: engine/third-party/install/SFML/
    # - install deps in: engine/third-party/install/SFML/deps/ (for Windows and OSX)
    # !! we are not currenty building project linking to deps/, so build will only work on Linux
    # !! where SFML deps are installed on system
    # - install misc in: engine/third-party/install/SFML/share/
    # - release build with CMAKE_BUILD_TYPE=Release (default)
    # - use architecture: x86_64 (default)
    # - no doc, no examples (default)
    # - no OpenGL ES (default)
    # - use deps in SFML/extlibs to reduce dependency on system libs (default) (for Windows and OSX)
    options = [
        "-G", "Unix Makefiles",
        "-DBUILD_SHARED_LIBS=FALSE",
        "-DSFML_BUILD_NETWORK=FALSE",
        f"-DCMAKE_INSTALL_PREFIX={sfml_install_path}",
        f"-DSFML_DEPENDENCIES_INSTALL_PREFIX={sfml_install_path}/deps",
        f"-DSFML_MISC_INSTALL_PREFIX={sfml_install_path}/share",
        "-DSFML_INSTALL_PKGCONFIG_FILES=FALSE",
    ]
    try:
        run(["cmake", *options, sfml_path], cwd=sfml_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"cmake command failed")
        sys.exit(e.returncode)

    cpu_count = 1
    try:
        cpu_count = multiprocessing.cpu_count()
    except NotImplementedError as e:
        logging.error(f"multiprocessing.cpu_count() not implemented, defaulting to -j1")

    try:
        run(["make", f"-j{cpu_count}"], cwd=sfml_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"make command failed")
        sys.exit(e.returncode)

    try:
        run(["make", "install"], cwd=sfml_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"make install command failed")
        sys.exit(e.returncode)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    # get third-party dir from this file's path
    third_party_path = Path(__file__).parent.absolute()
    sfml_path = third_party_path.joinpath("SFML")
    sfml_build_path = third_party_path.joinpath("build/SFML")
    sfml_install_path = third_party_path.joinpath("install/SFML")

    # build SFML in target path
    logging.info(f"Building SFML from {sfml_path} in {sfml_build_path} and installing to {sfml_install_path}...")
    build_sfml(sfml_path, sfml_build_path, sfml_install_path)
    logging.info("Done.")
