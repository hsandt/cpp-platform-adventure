#pragma once

// Game
#include "Common.h"
#include "Application/GameState.h"
#include "GameStates/GameStateID.h"

class InGameState : public GameState
{
public:

    explicit InGameState(GameApplication& gameApp);
    ~InGameState();

    /// Return game state ID from enum
    virtual constexpr u8 getID() const override { return (u8) GameStateID::InGame; }

    /// Callback on entering game state
    virtual void onEnter() override;

    /// Update in-game state
    virtual void update() override;

public:

    /* Parameters */

    std::string mp_initialSceneName;
};
