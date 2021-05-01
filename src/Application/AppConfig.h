#pragma once

// std
#include <string>

// Game
#include "Common.h"

namespace YAML
{
    class Node;
}

/// Application configuration
/// Read from YAML config file
struct AppConfig
{
    /// Parse YAML file containing app config and return AppConfig
    static AppConfig fromFile(const std::string& filename);

    /// Default constructor with sensible defaults
    AppConfig();

    /// Application FPS
    /// Default: 60
    u8 fps;

    /// Maximum number of updates done before a render
    /// When application is lagging, this allows
    /// the game to drop some frames to let physics catch up before the next render.
    /// If lag is so important that the extra updates are not enough to catch up, the game drops
    /// as many frames as it can (maxUpdatesPerRender - 1) *and* still slows down.
    /// Default: 3
    u8 maxUpdatesPerRender;

    /// Name of initial scene to load
    /// This is the path to the scene file's basename (no .yml), relative to assets/scenes.
    /// Default: "init"
    std::string initialSceneName;
};
