#pragma once

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"

class GameState : public ApplicationObject
{
public:

    explicit GameState(GameApplication& gameApp);
    ~GameState();

    /// Return game state ID (often a constant taken from an enum)
    virtual constexpr u8 getID() const = 0;

    /// Callback on entering game state (optional override)
    virtual void onEnter() {}

    /// Update game state
    virtual void update() = 0;

    /// Callback on exiting game state (optional override)
    virtual void onExit() {}
};
