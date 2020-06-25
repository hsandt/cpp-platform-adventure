# C++ Platform Adventure Game

A personal game dev project to train making a game in C++20 with low-level libraries, and gradually extracting components to make a more generic engine.

## Supported platforms

Only Linux is currently supported.

However, premake will make it easier to support Windows and OSX in a later version.

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

Note: `libc6-dev` is part of build-essential