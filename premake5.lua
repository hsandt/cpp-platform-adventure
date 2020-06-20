workspace "C++_Platform_Adventure"
    configurations { "Debug", "Release" }
    location "build"

project "Game"
    kind "ConsoleApp"
    language "C++"
    -- premake doesn't support cppdialect "C++20" yet,
    -- so use std option directly
    buildoptions "-std=c++20"

    includedirs { "engine/third-party/SFML/include" }
    libdirs {"engine/third-party/build/SFML/lib"}
    -- TODO WINDOWS & OSX: adapt libs below which are for Linux
    links {
        "sfml-graphics-s", "sfml-window-s", "sfml-audio-s", "sfml-system-s",
        -- static linking of SFML requires linking to their own dependencies
        -- it's basically the list on https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php
        -- without freetype, changing opengl -> GL (to have GLX functions) and uppercase FLAC
        "X11",
        "Xrandr",
        "udev",
        "GL",
        "FLAC",
        "ogg",
        "vorbis",
        "vorbisenc",
        "vorbisfile",
        "openal",
        "pthread"
    }

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
