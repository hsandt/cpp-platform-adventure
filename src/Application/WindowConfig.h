#pragma once

#include <string>

#include <SFML/System/String.hpp>

struct WindowConfig
{
    /// Parse YAML file containing window config and return WindowConfig
    static WindowConfig from_file(const std::string& filename);

    /// Default constructor with sensible defaults
    WindowConfig();

    /// Window dimensions (for VideoMode)
    unsigned int width;
    unsigned int height;

    // Framerate limit (for Window)
    unsigned int framerateLimit;

    /// Anti-aliasing level (for ContextSettings)
    unsigned int antialiasingLevel;

    // Window title (for Window)
    sf::String title;
};
