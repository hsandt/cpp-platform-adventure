#include "WindowConfig.h"

#include <iostream>
#include <string>

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
        YAML::Node windowConfigFile = YAML::LoadFile("config/window.yml");

        // implicit conversion to unsigned int
        windowConfig.width = windowConfigFile["width"].as<int>();
        windowConfig.height = windowConfigFile["height"].as<int>();
        windowConfig.framerateLimit = windowConfigFile["framerateLimit"].as<int>();
        windowConfig.antialiasingLevel = windowConfigFile["antialiasingLevel"].as<int>();

        // implicit conversion to sf::String
        windowConfig.title = windowConfigFile["title"].as<std::string>();
    }
    catch(const YAML::BadFile& e)
    {
        std::cerr << e.what() << " config/window.yml, using default window config.\n";
    }

    return windowConfig;
}
