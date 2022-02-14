#pragma once

// Game
#include "Common.h"
#include "Application/GameState.h"
#include "GameStates/GameStateID.h"

// RmlUi
namespace Rml
{
    class Element;
    class ElementDocument;
}

class TitleMenuState : public GameState
{
public:

    explicit TitleMenuState(GameApplication& gameApp);
    ~TitleMenuState();

    /// Return game state ID from enum
    virtual constexpr u8 getID() const override { return (u8) GameStateID::TitleMenu; }

    /// Callback on entering game state
    virtual void onEnter() override;

    /// Update game state
    virtual void update() override;

    /// Callback on exiting game state
    virtual void onExit() override;

private:

    /* External references */

    /// Title menu document
    SafePtr<Rml::ElementDocument> mr_titleMenuDocument;
};
