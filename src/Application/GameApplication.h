#pragma once

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Common.h"
#include "Memory/Box.hpp"

namespace sf
{
    class RenderWindow;
    class View;
}

class DialogueManager;
class InputManager;
class UICanvas;
class World;

/// Game Application. Handles window and game loop.
class GameApplication
{
public:
    GameApplication();
    virtual ~GameApplication();

public:

    /// Initialize and run game application
    /// Only method made public for safety.
    void init_and_run();

private:

    /// Initialize game application
    void init();

    /// Run game loop
    /// Returns when the loop is over, i.e. the window is closed.
    void run();

    /// Update the game
    void update(sf::Time deltaTime);

    /// Render the game
    void render();

public:

    /* Components */

    /// Render window
    const Box<sf::RenderWindow> mc_window;

    /// View used to draw in window
    const Box<sf::View> mc_view;

    /// Game world
    const Box<World> mc_world;

    /// Game UI
    const Box<UICanvas> mc_uiCanvas;

    /// Input manager
    const Box<InputManager> mc_inputManager;

    /// Dialogue manager
    const Box<DialogueManager> mc_dialogueManager;


    /* Parameters */

    /// Frame duration
    sf::Time mp_frameDuration;

private:

    /* State */

    /// Time elapsed in application since start
    /// Unlike real time, it only cumulates confirmed update delta times,
    /// not raw elapsed times, and is therefore a multiple a frame duration
    sf::Time m_applicationTime;
};
