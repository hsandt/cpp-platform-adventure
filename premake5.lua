-- ex: build/linux_gmake_clang_debug
-- don't rely on %{cfg.toolset}, it's 'gcc' even for clang
-- instead, define one configuration per pair (toolchain, debug/release)
local output_dir = "build/%{cfg.system}_%{cfg.action}_%{cfg.buildcfg}"

workspace "C++_Platform_Adventure"
    configurations { "gcc_debug", "gcc_release", "clang_debug", "clang_release" }
    location "build"
    targetdir(output_dir .. "/bin")
    objdir(output_dir .. "/obj")

project "Game"
    kind "WindowedApp"
    language "C++"
    -- premake doesn't support cppdialect "C++20" yet,
    -- so use std option directly
    buildoptions "-std=c++20"
    includedirs { "src" }
    flags { "FatalWarnings" }
    enablewarnings { "all" }

    -- Dependency: PPK_ASSERT --

    -- PPK_ASSERT is not a pre-built & linked library.
    -- Instead we directly add its header and source to the project and build them with the rest.
    includedirs { "engine/third-party/PPK_ASSERT/src" }
    files { "engine/third-party/PPK_ASSERT/src/**.h", "engine/third-party/PPK_ASSERT/src/**.cpp" }
    -- g++ release detects false positive warnings related to strncpy


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
            -- note that freetype is a dependency of both SFML Font system, and RmlUI
            "freetype",
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


    -- Dependency: RmlUi --

    includedirs { "engine/third-party/install/RmlUi/include" }
    libdirs {"engine/third-party/install/RmlUi/lib"}
    -- RmlDebugger is for debug only, see filter on configurations below
    links {"RmlCore"}


    -- add all files recursively to project
    files { "src/**.h", "src/**.cpp" }

    filter { "configurations:*_debug" }
        defines { "DEBUG" }
        symbols "On"
        buildoptions "-O0"
        -- this will also be copied with prebuildcommands for debug configs only
        links {"RmlDebugger"}

    filter { "configurations:*_release" }
        defines { "NDEBUG" }
        -- `optimize "On"` sets -O2 instead of -O3, so prefer manual options
        buildoptions "-O3"

    filter {}


    -- Copy assets and config folders + dynamic libraries + run script near executable
    -- Delete any existing folder to cleanup old files now removed from assets and config
    -- Note that paths are relative to location "build", so we access project root with ".." (else we'd need to use project_root = os.realpath("."))
    --   (%{wks.location} and %{prj.location} are both relative to location themselves, so ".")
    -- We prefer prebuild to postbuild, simply because in case of failure, the whole process fails and next time we try to build,
    --   it will also try to copy again (in postbuild, the executable has already been generated and `make` does nothing on next build,
    --   even if there are files to copy)
    -- OSX note: this is not the standard way to do it, we should Copy Bundle Resources into the .app.
    prebuildcommands {
        "{MKDIR} %{cfg.targetdir}",
        "{RMDIR} %{cfg.targetdir}/assets",
        "{RMDIR} %{cfg.targetdir}/config",
        -- when premake releases new version integrating https://github.com/premake/premake-core/pull/1528
        -- replace COPY with COPYDIR for clarity and Windows compatibility
        "{COPY} ../assets ../config %{cfg.targetdir}",
    }

    filter { "system:linux" }
        prebuildcommands {
            -- copy dynamic libraries required for playing
            "{COPY} ../engine/third-party/install/RmlUi/lib/libRmlCore.so* %{cfg.targetdir}",
            -- copy run script (allows to run the game from any working directory while still accessing assets,
            --  config and dynamic libraries copied along the executable)
            "{COPY} ../engine/dist_scripts/run.sh %{cfg.targetdir} && chmod +x %{cfg.targetdir}/run.sh",
        }
    filter { "system:linux", "configurations:*_debug" }
        prebuildcommands {
            -- copy dynamic libraries required for debugging
            "{COPY} ../engine/third-party/install/RmlUi/lib/libRmlDebugger.so* %{cfg.targetdir}",
        }
    filter {}
