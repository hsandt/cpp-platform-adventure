#include "WindowConfig.h"

#include <iostream>
#include <string>
#include <sstream>

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
        // what() just contains "bad file", so prefer custom message
        // TODO: use https://github.com/fmtlib/fmt to format string
        std::ostringstream oss;
        oss << "YAML::BadFile: '" << filename << "'" << std::endl;
        throw(std::runtime_error(oss.str()));
    }

    return windowConfig;
}

template<typename YAMLValue, typename Var, typename YAMLKey>
/* static */ bool WindowConfig::try_set_from_key(Var& var, const YAMLKey& key, const YAML::Node& windowConfigFile)
{
    YAML::Node node = windowConfigFile[key];
    if (node.IsDefined())
    {
        // implicit conversion from Value to Out type may happen
        // TODO: use concepts with convertible_to
        var = node.as<YAMLValue>();
        return true;
    }

    return false;
}
