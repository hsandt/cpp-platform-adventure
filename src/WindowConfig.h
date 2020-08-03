#pragma once

#include <bits/stringfwd.h>
#include <concepts>

#include <SFML/System/String.hpp>

namespace YAML
{
    class Node;
}

struct WindowConfig
{
    /// Default constructor with sensible defaults
    WindowConfig();

    /// Parse YAML file containing window config and return WindowConfig
    static WindowConfig from_file(const std::string& filename);

    /// Search for key in windowConfigFile
    /// If key is found, set var by reference to YAML value at key and return true
    /// Else, return false
    /// YAMLValue must be convertible to Var type
    /// Var and YAMLKey types can be deduced from the arguments, so just pass <YAMLValue> on call
    /// (int, std::string, etc.)
    template<typename YAMLValue, typename Var, typename YAMLKey>
    requires std::convertible_to<YAMLValue, Var>
    static bool try_set_from_key(Var& var, const YAMLKey& key, const YAML::Node& windowConfigFile);

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
