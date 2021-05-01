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

    /// Default constructor with sensible defaults
    AppConfig();

    /// FPS
    u8 fps;

    /// Maximum number of updates done before a render. There are 2 updates or more so physics can
    /// catch up during lag. When reaching more than the max, the game slows down instead of
    /// dropping frames.
    u8 maxUpdatesPerRender;
};
