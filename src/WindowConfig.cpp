#include "WindowConfig.h"

#include <iostream>
#include <string>
#include <sstream>

#include "fmt/format.h"
#include "yaml-cpp/yaml.h"

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

/* static */ WindowConfig WindowConfig::from_file(const std::string& filename)
{
    WindowConfig windowConfig;

    try
    {
        YAML::Node windowConfigFile = YAML::LoadFile(filename);

        try_set_from_key<int>(windowConfig.width, "width", windowConfigFile);
        try_set_from_key<int>(windowConfig.height, "height", windowConfigFile);
        try_set_from_key<int>(windowConfig.framerateLimit, "framerateLimit", windowConfigFile);
        try_set_from_key<int>(windowConfig.antialiasingLevel, "antialiasingLevel", windowConfigFile);
        try_set_from_key<std::string>(windowConfig.title, "title", windowConfigFile);
    }
    catch(const YAML::BadFile& e)
    {
        // what() just contains "bad file", so prefer custom error message
        throw(std::runtime_error(fmt::format("YAML::BadFile: '{}'", filename)));
    }

    return windowConfig;
}

template<typename YAMLValue, typename Var, typename YAMLKey>
requires std::convertible_to<YAMLValue, Var>
/* static */ bool WindowConfig::try_set_from_key(Var& var, const YAMLKey& key, const YAML::Node& windowConfigFile)
{
    YAML::Node node = windowConfigFile[key];
    if (node.IsDefined())
    {
        // implicit conversion from Value to Out type may happen here
        var = node.as<YAMLValue>();
        return true;
    }

    return false;
}
