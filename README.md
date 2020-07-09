# C++ Platform Adventure Game

A personal game dev project to train making a game in C++20 with low-level libraries, and gradually extracting components to make a more generic engine.

## Changelog

See [CHANGELOG.md].

## Supported platforms

Only Linux is currently supported. Build is tested on Ubuntu and Mint, and probably works with other Debian-based distributions.

However, premake will make it easier to support Windows and OSX in a later version.

## Required compiler toolchain

This project will use C++20 (in particular concepts), so to build it you will need either GCC 10 (`g++-10`), LLVM 10/11 (`clang++-10` or just `clang++` on OSX) or Visual Studio 2019 (16.3+) depending on your platform.

In addition, you need `premake` to build the game project itself, and `cmake` to build the SFML submodule.

Premake binaries are available on the [official download page](https://premake.github.io/download.html).

You can either install `cmake` via a package manager (including chocolatey and scoop for Windows) or from the [official download page](https://cmake.org/download/).

Currently, only the current configurations are tested in CI:

- Linux Ubuntu with `g++-10`
- Linux Ubuntu with `clang++-10`
- OSX with `clang++` 11 (not passing yet)

You are free to install the toolchains as you wish as long as they are accessible in the `PATH`. Below I give a few instructions for supported platforms.

### Linux Ubuntu with g++-10

```shell
$ sudo add-apt-repository ppa:ubuntu-toolchain-r/test
$ sudo apt-get update
$ sudo apt install g++-10
```

### Linux Ubuntu with latest stable LLVM (currently 10)

Follow the instructions on https://apt.llvm.org/.

There is an install script for Debian and Ubuntu, and instructions to install the packages for Debian-based distributions.

### OSX with latest LLVM (currently 11)

On OSX, I recommend to install the latest Xcode (currently 11) and the lastest clang (currently 11) will be provided.

## Dependencies

### Linux

To build SFML, you need the libraries mentioned in https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php. I modified the list slightly to match the libraries I actually needed in [premake5.lua]'s `links` (in particular, I use GL instead of OpenGL to include GLX/GLExtensions). Exact names may vary depending on your distribution, I'll give the name of Debian packages below.

| Library    | Debian package |
|------------|----------------|
| X11        | libx11-dev     |
| Xrandr     | libxrandr-dev  |
| udev       | libudev-dev    |
| GL         | libgl-dev      |
| FLAC       | libflac-dev    |
| ogg        | libogg-dev     |
| vorbis     | libvorbis-dev  |
| vorbisenc  | libvorbisenc2  |
| vorbisfile | libvorbisfile3 |
| openal     | libopenal-dev  |
| pthread    | libc6-dev      |

Notes:

- for virtual package `libgl-dev`, install `libgl1-mesa-dev`
- `libc6-dev` is part of `build-essential`

Here is a one-liner you can use for a quick setup:

```shell
$ sudo apt install build-essential libx11-dev libxrandr-dev libudev-dev libgl1-mesa-dev libflac-dev libogg-dev libvorbis-dev libvorbisenc2 libvorbisfile3 libopenal-dev
```
