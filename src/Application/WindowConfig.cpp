#include "WindowConfig.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Serialization/YamlHelper.hpp"

/* static */ WindowConfig WindowConfig::fromFile(const std::string& filename)
{
    WindowConfig windowConfig;

    try
    {
        YAML::Node windowConfigFile = YAML::LoadFile(filename);

        // Same comment as in AppConfig::fromFile, but for <int> instead of <u16>
        YamlHelper::tryGet<int>(windowConfigFile, "nativeWidth", windowConfig.nativeWidth);
        YamlHelper::tryGet<int>(windowConfigFile, "nativeHeight", windowConfig.nativeHeight);
        YamlHelper::tryGet<int>(windowConfigFile, "upscaleFactor", windowConfig.upscaleFactor);
        YamlHelper::tryGet<bool>(windowConfigFile, "vsync", windowConfig.vsync);
        YamlHelper::tryGet<int>(windowConfigFile, "framerateLimit", windowConfig.framerateLimit);
        YamlHelper::tryGet<bool>(windowConfigFile, "allowVsyncWithFramerateLimit", windowConfig.allowVsyncWithFramerateLimit);
        YamlHelper::tryGet<int>(windowConfigFile, "antialiasingLevel", windowConfig.antialiasingLevel);
        YamlHelper::tryGet<std::string>(windowConfigFile, "title", windowConfig.title);
    }
    catch (const YAML::BadFile& e)
    {
        // what() just contains "bad file", so prefer assert with custom message
        PPK_ASSERT_DEBUG(false, "YAML::BadFile: '%s'", filename.c_str());
    }

    return windowConfig;
}

WindowConfig::WindowConfig() :
    // 720p
    nativeWidth(1280),
    nativeHeight(720),
    // no upscaling
    upscaleFactor(1),
    // V-sync false by default so framerate limit is used
    vsync(false),
    // Framerate limit at 60 for roughly 60 FPS
    framerateLimit(60),
    // V-sync is false by default, so this value will be ignored by default
    allowVsyncWithFramerateLimit(false),
    // No anti-aliasing
    antialiasingLevel(0),
    // Generic title
    title("Game")
{
}
