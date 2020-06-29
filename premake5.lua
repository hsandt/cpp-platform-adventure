workspace "C++_Platform_Adventure"
    configurations { "Debug", "Release" }
    location "build"

project "Game"
    kind "ConsoleApp"
    language "C++"
    -- premake doesn't support cppdialect "C++20" yet,
    -- so use std option directly
    -- g++-10 and clang++-10 support "c++20" on Linux, but clang++ 11 on OSX only uses "c++2a"
    buildoptions "-std=c++2a"

    includedirs { "engine/third-party/SFML/include" }
    libdirs {"engine/third-party/build/SFML/lib"}

    -- link to static SFML libs
    links {"sfml-graphics-s", "sfml-window-s", "sfml-audio-s", "sfml-system-s"}

    -- static linking of SFML requires linking to their own dependencies

    filter { "system:macosx" }
        -- on OSX, SFML deps are pre-installed in the repository, so just use them
        frameworkdirs {"engine/third-party/SFML/extlibs/libs-osx/Frameworks"}
        links {
            "FLAC.framework",
            "ogg.framework",
            "OpenAL.framework",
            "vorbis.framework",
            "vorbisenc.framework",
            "vorbisfile.framework"
        }

    filter { "system:linux" }
        -- on Linux, we basically use the list on https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php
        -- without freetype, changing opengl -> GL (to have GLX functions) and uppercase FLAC
        -- they must be installed locally on the machine
        links {
            "X11",
            "Xrandr",
            "udev",
            "GL",
            "pthread",
            "FLAC",
            "ogg",
            "openal",
            "vorbis",
            "vorbisenc",
            "vorbisfile"
        }        

    filter {}

    -- add all files recursively to project
    files { "src/**.h", "src/**.cpp" }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"
        buildoptions "-O0"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        -- `optimize "On"` sets -O2 instead of -O3, so prefer manual options
        buildoptions "-O3"
    
    filter {}
