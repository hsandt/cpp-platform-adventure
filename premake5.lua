workspace "C++_Platform_Adventure"
    configurations { "Debug", "Release" }
    location "build"

project "Game"
    kind "ConsoleApp"
    -- clang++-10 must be installed
    toolset "clang"
    language "C++"
    -- premake doesn't support cppdialect "C++20" yet,
    -- so use std option directly
    buildoptions "-std=c++20"

    -- add all files recursively to project
    files { "**.h", "**.cpp" }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
    
    filter {}