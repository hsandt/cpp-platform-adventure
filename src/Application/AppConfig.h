#pragma once

// std
#include <concepts>
#include <string>

// SFML
#include <SFML/System/String.hpp>

// Game
#include "Common.h"

namespace YAML
{
    class Node;
}

struct AppConfig
{
    /// Parse YAML file containing app config and return AppConfig
    static AppConfig fromFile(const std::string& filename);

    /// Search for key in appConfigFile
    /// If key is found, set var by reference to YAML value at key and return true
    /// Else, return false
    /// YAMLValue must be convertible to Var type
    /// Var and YAMLKey types can be deduced from the arguments, so just pass <YAMLValue> on call
    /// (int, std::string, etc.)
    template<typename YAMLValue, typename Var, typename YAMLKey>
    requires std::convertible_to<YAMLValue, Var>
    static bool trySetFromKey(Var& var, const YAMLKey& key, const YAML::Node& appConfigFile);

    /// Default constructor with sensible defaults
    AppConfig();

    /// FPS
    u8 fps;

    /// Maximum number of updates done before a render. There are 2 updates or more so physics can
    /// catch up during lag. When reaching more than the max, the game slows down instead of
    /// dropping frames.
    u8 maxUpdatesPerRender;
};
