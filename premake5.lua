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
    links {"sfml-graphics", "sfml-window", "sfml-system", "sfml-audio"}

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
