workspace "C++_Platform_Adventure"
    configurations { "Debug", "Release" }
    location "build"

project "Game"
    kind "WindowedApp"
    language "C++"
    -- premake doesn't support cppdialect "C++20" yet,
    -- so use std option directly
    -- g++-10 and clang-10 support "c++20" on Linux, but clang 11 on OSX only uses "c++2a"
    buildoptions "-std=c++2a"

    includedirs { "engine/third-party/install/SFML/include" }
    libdirs {"engine/third-party/install/SFML/lib"}

    -- link to static SFML libs (for GCC compatibility, make sure to put dependent libs
    -- before dependees)
    defines { "SFML_STATIC" }
    links {"sfml-graphics-s", "sfml-window-s", "sfml-audio-s", "sfml-system-s"}

    -- static linking of SFML requires linking to their own dependencies

    filter { "system:windows" }
        libdirs {"engine/third-party/SFML/extlibs/libs-msvc/x64"}
        links {
            "flac",
            "ogg",
            "openal32",
            "vorbis",
            "vorbisenc",
            "vorbisfile"
        }

    -- on OSX, SFML deps are pre-installed in the repository, so just use them
    -- frameworkdirs will work with Xcode only, so for gmake pass -F manually
    -- https://github.com/premake/premake-core/issues/196
    
    filter { "system:macosx", "action:xcode*"}
        frameworkdirs {"engine/third-party/SFML/extlibs/libs-osx/Frameworks"}

    filter { "system:macosx", "action:gmake*"}
        -- unlike ...dirs {}, linkoptions does not interpret paths
        -- so we must enter them relatively to location "build", hence "../"
        linkoptions {"-F ../engine/third-party/SFML/extlibs/libs-osx/Frameworks"}
        -- to avoid undefined symbols from various frameworks used by Xcode, we link those below:
        links {
            "AppKit.framework",  -- includes CoreFoundation, CoreGraphics, etc.
            "Carbon.framework",  -- old, but required for _LM, _TIS and _kTIS symbols
            "IOKit.framework"
        }

    filter { "system:macosx" }
        links {
            "OpenGL.framework",
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
