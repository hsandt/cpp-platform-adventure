#include "WindowConfig.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Serialization/YamlHelper.h"

/* static */ WindowConfig WindowConfig::fromFile(const std::string& filename)
{
    WindowConfig windowConfig;

    try
    {
        YAML::Node windowConfigFile = YAML::LoadFile(filename);

        YamlHelper::tryGet<int>(windowConfigFile, "width", windowConfig.width);
        YamlHelper::tryGet<int>(windowConfigFile, "height", windowConfig.height);
        YamlHelper::tryGet<bool>(windowConfigFile, "vsync", windowConfig.vsync);
        YamlHelper::tryGet<int>(windowConfigFile, "framerateLimit", windowConfig.framerateLimit);
        YamlHelper::tryGet<bool>(windowConfigFile, "allowVsyncWithFramerateLimit", windowConfig.allowVsyncWithFramerateLimit);
        YamlHelper::tryGet<int>(windowConfigFile, "antialiasingLevel", windowConfig.antialiasingLevel);
        YamlHelper::tryGet<std::string>(windowConfigFile, "title", windowConfig.title);
    }
    catch(const YAML::BadFile& e)
    {
        // what() just contains "bad file", so prefer custom error message
        throw std::runtime_error(fmt::format("YAML::BadFile: '{}'", filename));
    }

    return windowConfig;
}

WindowConfig::WindowConfig() :
    // 720p
    width(1280),
    height(720),
    // V-sync false by default so framerate limit is used
    vsync(false),
    // Framerate limit at 60 for roughly 60 FPS
    framerateLimit(60),
    // V-sync false by default, so this value will be ignored by default
    allowVsyncWithFramerateLimit(false),
    // No anti-aliasing
    antialiasingLevel(0),
    // Generic title
    title("Game")
{
}
