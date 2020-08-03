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

WindowConfig WindowConfig::from_file(const std::string& filename)
{
    WindowConfig windowConfig;

    try
    {
        YAML::Node windowConfigFile = YAML::LoadFile(filename);

        YAML::Node windowConfigFileWidth = windowConfigFile["width"];
        if (windowConfigFileWidth.IsDefined())
        {
            // implicit conversion to unsigned int
            windowConfig.width = windowConfigFileWidth.as<int>();
        }

        YAML::Node windowConfigFileHeight = windowConfigFile["height"];
        if (windowConfigFileHeight.IsDefined())
        {
            // implicit conversion to unsigned int
            windowConfig.height = windowConfigFileHeight.as<int>();
        }

        YAML::Node windowConfigFileFramerateLimit = windowConfigFile["framerateLimit"];
        if (windowConfigFileFramerateLimit.IsDefined())
        {
            // implicit conversion to unsigned int
            windowConfig.framerateLimit = windowConfigFileFramerateLimit.as<int>();
        }

        YAML::Node windowConfigFileAntialiasingLevel = windowConfigFile["antialiasingLevel"];
        if (windowConfigFileAntialiasingLevel.IsDefined())
        {
            // implicit conversion to unsigned int
            windowConfig.antialiasingLevel = windowConfigFileAntialiasingLevel.as<int>();
        }

        YAML::Node windowConfigFileTitle = windowConfigFile["title"];
        if (windowConfigFileTitle.IsDefined())
        {
            // implicit conversion to sf::String
            windowConfig.title = windowConfigFileTitle.as<std::string>();
        }
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
