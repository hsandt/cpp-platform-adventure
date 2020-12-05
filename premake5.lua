-- ex: build/Linux_debug
output_dir = "build/%{cfg.system}_%{cfg.action}_%{cfg.buildcfg}"

workspace "C++_Platform_Adventure"
    configurations { "Debug", "Release" }
    location "build"
    targetdir(output_dir .. "/bin")
    objdir(output_dir .. "/obj")

project "Game"
    kind "WindowedApp"
    language "C++"
    -- premake doesn't support cppdialect "C++20" yet,
    -- so use std option directly
    -- g++-10 and clang-10 support "c++20" on Linux, but clang 11 on OSX only uses "c++2a"
    buildoptions "-std=c++2a"
    includedirs { "src" }
    flags { "FatalWarnings" }
    enablewarnings { "all" }

    -- Dependency: {fmt} --

    includedirs { "engine/third-party/install/fmt/include" }
    libdirs {"engine/third-party/install/fmt/lib"}
    links {"fmt"}


    -- Dependency: SFML --

    -- Unfortunately, SFML uses the convention of including its headers with <> instead of ""
    -- gmake will tolerate this but Xcode will reject USER library headers include with <> unless
    -- ALWAYS_SEARCH_USER_PATHS is YES (deprecated, and hardcoded to NO in premake anyway)
    -- or we are using a framework (only possible when building SFML dynamically).
    -- So, when building SFML statically, to support Xcode we need to use sysincludedirs
    -- instead of includedirs to set HEADER_SEARCH_PATHS instead of USER_SEARCH_PATHS in Xcode.
    sysincludedirs { "engine/third-party/install/SFML/include" }
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
        -- it seems that even with gmake, OSX builds an .app, so no need to add target suffix "_OSX"

        -- unlike ...dirs {}, linkoptions does not interpret paths
        -- so we must enter them relatively to location "build", hence "../"
        linkoptions {"-F ../engine/third-party/SFML/extlibs/libs-osx/Frameworks"}

    filter { "system:macosx" }
        -- to avoid undefined symbols from various frameworks used by Xcode, we link those below:
        links {
            -- SFML's own dependencies, OSX-specific
            "AppKit.framework",  -- includes CoreFoundation, CoreGraphics, etc.
            "Carbon.framework",  -- old, but required for _LM, _TIS and _kTIS symbols
            "IOKit.framework",
            -- SFML's own dependencies, generic
            "OpenGL.framework",
            "FLAC.framework",
            "ogg.framework",
            "OpenAL.framework",
            "vorbis.framework",
            "vorbisenc.framework",
            "vorbisfile.framework"
        }

    filter { "system:linux" }
        -- only Linux can generate extensionless executables, but we keep the target suffix
        -- we prepared when we thought OSX gmake would also build one, for now
        targetsuffix "_Linux"

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


    -- Dependency: yaml-cpp --

    includedirs { "engine/third-party/install/yaml-cpp/include" }
    libdirs {"engine/third-party/install/yaml-cpp/lib"}
    links {"yaml-cpp"}

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


    -- Copy assets and config folders near executable
    -- Note that paths are relative to location "build", so we access project root with ".." (else we'd need to use project_root = os.realpath("."))
    --   (%{wks.location} and %{prj.location} are both relative to location themselves, so ".")
    -- We prefer prebuild to postbuild, simply because in case of failure, the whole process fails and next time we try to build,
    --   it will also try to copy again (in postbuild, the executable has already been generated and `make` does nothing on next build,
    --   even if there are files to copy)
    -- OSX note: this is not the standard way to do it, we should Copy Bundle Resources into the .app.
    prebuildcommands {
        "{MKDIR} %{cfg.targetdir}",
        "{COPY} ../assets ../config %{cfg.targetdir}",
    }
