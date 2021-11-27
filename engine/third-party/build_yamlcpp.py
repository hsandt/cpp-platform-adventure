#!/usr/bin/env python3

# Build and install yaml-cpp

import sys
import logging
from pathlib import Path
from subprocess import run, CalledProcessError
import multiprocessing

# Version check
if sys.version_info.minor < 6:
    print("Python version is %s, 3.6+ is required." % sys.version)
    sys.exit(1)


def build_yamlcpp(yamlcpp_path: Path, yamlcpp_build_path: Path, yamlcpp_install_path: Path):
    """Build yaml-cpp from source path into build path"""
    # create directory with any intermediate parents, if needed
    # similar to Unix: mkdir -p
    Path(yamlcpp_build_path).mkdir(parents=True, exist_ok=True)

    # We want:
    # - static lib with YAML_BUILD_SHARED_LIBS=OFF (default)
    # - release build with CMAKE_BUILD_TYPE=Release (default)
    # - install with YAML_CPP_INSTALL=ON (default)
    # - install locally in: engine/third-party/install/yaml-cpp/
    # - no contrib, tools, tests for fastest and smallest build possible
    options = [
        "-G", "Unix Makefiles",
        f"-DCMAKE_INSTALL_PREFIX={yamlcpp_install_path}",
        f"-DYAML_CPP_BUILD_CONTRIB=OFF",
        f"-DYAML_CPP_BUILD_TOOLS=OFF",
        f"-DYAML_CPP_BUILD_TESTS=OFF",
    ]
    try:
        run(["cmake", *options, yamlcpp_path], cwd=yamlcpp_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"cmake command failed")
        sys.exit(e.returncode)

    cpu_count = 1
    try:
        cpu_count = multiprocessing.cpu_count()
    except NotImplementedError as e:
        logging.error(f"multiprocessing.cpu_count() not implemented, defaulting to -j1")

    try:
        run(["make", f"-j{cpu_count}"], cwd=yamlcpp_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"make command failed")
        sys.exit(e.returncode)

    try:
        run(["make", "install"], cwd=yamlcpp_build_path, check=True)
    except CalledProcessError as e:
        logging.error(f"make install command failed")
        sys.exit(e.returncode)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    # get third-party dir from this file's path
    third_party_path = Path(__file__).parent.absolute()
    yamlcpp_path = third_party_path.joinpath("yaml-cpp")
    yamlcpp_build_path = third_party_path.joinpath("build/yaml-cpp")
    yamlcpp_install_path = third_party_path.joinpath("install/yaml-cpp")

    # build yaml-cpp in target path
    logging.info(f"Building yaml-cpp from {yamlcpp_path} in {yamlcpp_build_path} and installing to {yamlcpp_install_path}...")
    build_yamlcpp(yamlcpp_path, yamlcpp_build_path, yamlcpp_install_path)
    logging.info("Done.")
