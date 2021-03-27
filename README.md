[![Build Status: master](https://travis-ci.org/hsandt/cpp-platform-adventure.svg?branch=master)](https://travis-ci.org/hsandt/cpp-platform-adventure)
[![Build Status: develop](https://travis-ci.org/hsandt/cpp-platform-adventure.svg?branch=develop)](https://travis-ci.org/hsandt/cpp-platform-adventure)

# C++ Platform Adventure Game

A personal game dev project to train making a game in C++20 with low-level libraries, and gradually extracting components to make a more generic engine.

## Changelog

See [CHANGELOG](CHANGELOG.md).

## Supported platforms

Only Linux is currently supported. Build is tested on Ubuntu and Mint, and probably works with other Debian-based distributions.

However, premake will make it easier to support Windows and OSX in a later version.

## How to build

### Required compiler toolchain

This project will use C++20 (in particular concepts), so to build it you will need either GCC 10 (`g++-10`), LLVM 10/11 (`clang++-10` or just `clang++` on OSX) or Visual Studio 2019 (16.3+) depending on your platform.

In addition, you need `premake` to build the game project itself, and `cmake` to build the SFML submodule.

Premake binaries are available on the [official download page](https://premake.github.io/download.html).

You can either install `cmake` via a package manager (including chocolatey and scoop for Windows) or from the [official download page](https://cmake.org/download/).

Currently, only the current configurations are tested in CI:

- Linux Ubuntu with `g++-10`
- Linux Ubuntu with `clang++-10`
- OSX with `clang++` 11 (not passing yet)

You are free to install the toolchains as you wish as long as they are accessible in the `PATH`. Below I give a few instructions for supported platforms.

#### Linux Ubuntu with g++-10

```shell
$ sudo add-apt-repository ppa:ubuntu-toolchain-r/test
$ sudo apt-get update
$ sudo apt install g++-10
```

#### Linux Ubuntu with latest stable LLVM (currently 10)

Follow the instructions on https://apt.llvm.org/.

There is an install script for Debian and Ubuntu, and instructions to install the packages for Debian-based distributions.

#### OSX with latest LLVM (currently 11)

On OSX, I recommend to install the latest Xcode (currently 11) and the lastest clang (currently 11) will be provided.

### Dependencies

You need python3.6+ (named `python3` in your PATH) to run build_sfml.py.

#### Linux

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

### Building commands

Once you have installed the toolchains and dependencies mentioned above, you can start compiling the project.

The [Travis config](.travis.yml) is a good reference for the building procedure on Linux and OSX, line by line. It also refers to some of the toolchains and dependencies mentioned above (although fewer, since Travis images are pre-bundled with a number of libraries).

If you use VS Code, you can directly use the [tasks](.vscode/tasks.json).

Otherwise, following the instructions below.

#### gmake (Linux, OSX)

From the repository root:

```shell
# build third-party dependencies
$ engine/third-party/build_sfml.py
# build the game
$ premake gmake
$ config=release make -j2
```

The game executable will be created at build/bin/Release/Game.

#### Xcode (OSX, experimental)

! The game is known not to currently build on OSX due to OSX's clang not supporting `<concepts>`.

From the repository root:

```shell
# build third-party dependencies
$ engine/third-party/build_sfml.py
# build the game
$ premake xcode4
$ xcodebuild -project Game.xcodeproj -target Game -configuration Release
```

The game executable will be created at build/bin/Release/Game.

## Coding conventions

Class headers and sources follow [ClassTemplate.h](doc/ClassTemplate.h) and [ClassTemplate.cpp](doc/ClassTemplate.cpp) respectively.

### Variable naming

We do not use Hungarian notation. Members have their own prefixes depending on their roles:

- mo\_ for owning objects (directly or via a chain of owners). They are guaranteed to live,
so they are stored as references.
- mc\_ for components
- mp\_ for parameters (not pointers)
- ms\_ for state variables (static members are just s\_)

## Credits

### Free BGM

- bgm1.ogg from 傾き屋根から見た景色民 (2009-03-07) by mozeen ([free music DL page](https://mozeen.com/music/free_music_dl/)). Extracted first section (0.2s - 58.784s) to make it loop. Converted to ogg with quality 0 to make it work with SFML and still be lightweight.
