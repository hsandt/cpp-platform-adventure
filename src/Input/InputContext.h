#pragma once

// Game
#include "Common.h"

enum InputContext : u8
{
    /// Player character is moving around the scene
    Platforming,

    /// Player character is talking to an NPC, reading a sign post, etc.
    Dialogue
};
