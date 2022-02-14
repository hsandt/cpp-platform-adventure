#include "InGameState.h"

// Game
#include "Application/GameApplication.h"
#include "Space/World.h"
#include "Input/InputManager.h"

InGameState::InGameState(GameApplication& gameApp) :
    GameState(gameApp)
{
}

InGameState::~InGameState()
{
}

void InGameState::onEnter()
{
    // load initial scene
    mo_gameApp.mc_world->deferLoadScene(mp_initialSceneName);

    // set initial input context to Platforming
    mo_gameApp.mc_inputManager->pushInputContext(InputContext::Platforming);

    // set background color to sky color
    mo_gameApp.mc_world->mp_backgroundColor = sf::Color::Cyan;
}

void InGameState::update()
{
}
