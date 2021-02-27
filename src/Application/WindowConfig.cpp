#include "WindowConfig.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Serialization/YamlHelper.hpp"

/* static */ WindowConfig WindowConfig::from_file(const std::string& filename)
{
    WindowConfig windowConfig;

    try
    {
        YAML::Node windowConfigFile = YAML::LoadFile(filename);

        YamlHelper::tryGet<int>(windowConfig.width, "width", windowConfigFile);
        YamlHelper::tryGet<int>(windowConfig.height, "height", windowConfigFile);
        YamlHelper::tryGet<int>(windowConfig.framerateLimit, "framerateLimit", windowConfigFile);
        YamlHelper::tryGet<int>(windowConfig.antialiasingLevel, "antialiasingLevel", windowConfigFile);
        YamlHelper::tryGet<std::string>(windowConfig.title, "title", windowConfigFile);
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
    // Framerate limit at 60 for roughly 60 FPS
    framerateLimit(60),
    // No anti-aliasing
    antialiasingLevel(0),
    // Generic title
    title("Game")
{
}
