#include "GameStateManager.h"

GameStateManager::GameStateManager(GameApplication& gameApp) :
    ApplicationObject(gameApp)
{
}

GameStateManager::~GameStateManager()
{
}

void GameStateManager::update()
{
    // Check for next game state to enter
    if (ms_oNextState)
    {
        enterGameState(*ms_oNextState);
        ms_oNextState = nullptr;
    }

    // Update current state, which may have changed in the block above
    if (ms_oCurrentState)
    {
        ms_oCurrentState->update();
    }
}

void GameStateManager::addGameState(std::unique_ptr<GameState> gameState)
{
    if (gameState)
    {
        mp_gameStateMap.emplace(gameState->getID(), std::move(gameState));
    }
    else
    {
        PPK_ASSERT_DEBUG(false, "Cannot add empty game state");
    }
}

void GameStateManager::queryEnterGameState(u8 gameStateID)
{
    // don't return early nor assert if gameStateID is the current game state's ID,
    // to allow restarting the current game state
    auto it = mp_gameStateMap.find(gameStateID);
    if (it != mp_gameStateMap.end())
    {
        ms_oNextState = it->second.get();
    }
    else
    {
        PPK_ASSERT_DEBUG(false, "Cannot query game state with ID: %u, no such game state "
            "found in the game state map", gameStateID);
    }
}

void GameStateManager::enterGameState(GameState& gameState)
{
    // if a game state was already running, call exit callback
    if (ms_oCurrentState)
    {
        ms_oCurrentState->onExit();
    }

    // set new game state
    ms_oCurrentState = &gameState;

    // call enter callback
    gameState.onEnter();
}
