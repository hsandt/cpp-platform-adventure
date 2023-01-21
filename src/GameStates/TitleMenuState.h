#pragma once

// RmlUi
#include <RmlUi/Core/Types.h>

// Game
#include "Common.h"
#include "Application/GameState.h"
#include "GameStates/GameStateID.h"

// RmlUi
namespace Rml
{
    // RmlUi/Core/Types.h forwards common types, so just forward the less common ones
    class ElementDocument;
}

#include <RmlUi/Core/DataModelHandle.h>

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

    /// Start a new game
    void startGame(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);

private:

    /* External references */

    /// Title menu document
    SafePtr<Rml::ElementDocument> mr_titleMenuDocument;
};
