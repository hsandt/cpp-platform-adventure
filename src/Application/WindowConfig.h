#pragma once

// std
#include <string>

// Game
#include "Common.h"

struct WindowConfig
{
    /// Parse YAML file containing window config and return WindowConfig
    static WindowConfig fromFile(const std::string& filename);

    /// Default constructor with sensible defaults
    WindowConfig();

    /// Window dimensions (for VideoMode)
    u16 width;
    u16 height;

    /// Integer render upscaling factor (for games using pixel art)
    u8 upscaleFactor;

    /// V-sync enabling flag. If true, framerateLimit is ignored (for Window)
    bool vsync;

    /// Framerate limit (for Window)
    u16 framerateLimit;

    // Experimental: set framerate limit even when V-sync is enabled (for Window)
    // This is discouraged by SFML doc, but source code shows that this is equivalent to sleeping
    // until next frame after every render. Sleeping in Window::display is done without margin
    // though, so consider setting framerateLimit slightly higher than app FPS.
    bool allowVsyncWithFramerateLimit;

    /// Anti-aliasing level (for ContextSettings)
    u8 antialiasingLevel;

    // Window title (for Window)
    sf::String title;
};
