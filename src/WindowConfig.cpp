#include "WindowConfig.h"

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
