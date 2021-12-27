#pragma once

// std
#include <functional>
#include <map>
#include <optional>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"
#include "Application/GameState.h"
#include "Memory/Box.hpp"
#include "Memory/SafePtr.hpp"

class GameStateManager : public ApplicationObject
{
public:

    explicit GameStateManager(GameApplication& gameApp);
    ~GameStateManager();

    /// Add game state to the map of game states
    /// This moves ownership from the unique pointer to a Box in mp_gameStateMap.
    /// UB unless gameState is not empty, and there is not already a game state with the same ID.
    void addGameState(std::unique_ptr<GameState> gameState);

    /// Enter any next game state, then update (possibly new) current game state, if any
    void update();

    /// Query enter to next game state on next frame
    void queryEnterGameState(u8 gameStateID);

private:

    /// Enter another game state
    void enterGameState(GameState& gameState);

private:

    /* Parameters */

    /// Map of owned game states, indexed by ID
    /// We don't index by enum to make it compatible with different games using
    /// different game states, but you can store a game-specific enum of game states
    /// in a different file, and convert the enum value to u8 for convenience.
    std::map<u8, Box<GameState>> mp_gameStateMap;


    /* State */

    /// Current game state, if any
    SafePtr<GameState> ms_oCurrentState;

    /// Next game state to enter, if any
    SafePtr<GameState> ms_oNextState;
};
