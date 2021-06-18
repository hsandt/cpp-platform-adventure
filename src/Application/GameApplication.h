#pragma once

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Common.h"
#include "Memory/Box.hpp"

namespace sf
{
    class RenderTexture;
    class RenderWindow;
    class View;
}

class DialogueManager;
class GameStateManager;
class InputManager;
class MusicManager;
class TextureManager;
class UICanvas;
class World;

/// Game Application. Handles window and game loop.
class GameApplication
{
public:

    GameApplication();
    ~GameApplication();

    /// Initialize and run game application
    void initAndRun();

private:

    /// Initialize game application
    void init();

    /// Initialize window, render texture and view
    void initWindow();

    /// Initialize game state manager
    void initGameStateManager();

    /// Run game loop
    /// Returns when the loop is over, after closing the window.
    void run();

    /// Update the game by deltaTime
    void update(sf::Time deltaTime);

    /// Render the game
    void render();

public:

    /* Components */

    /// Render window
    const Box<sf::RenderWindow> mc_window;

    /// Render texture drawn (upscaled) on render window
    const Box<sf::RenderTexture> mc_renderTexture;

    /// View used to draw in window
    const Box<sf::View> mc_view;

    /// Game world
    const Box<World> mc_world;

    /// Game UI canvas
    const Box<UICanvas> mc_uiCanvas;

    /// GameState manager
    const Box<GameStateManager> mc_gameStateManager;

    /// Input manager
    const Box<InputManager> mc_inputManager;

    /// Texture manager
    const Box<TextureManager> mc_textureManager;

    /// Audio manager
    const Box<MusicManager> mc_musicManager;

    /// Dialogue manager
    const Box<DialogueManager> mc_dialogueManager;


    /* Parameters */

    /// Frame duration
    sf::Time mp_frameDuration;

    /// Maximum number of updates done before a render. See AppConfig::maxUpdatesPerRender.
    u8 mp_maxUpdatesPerRender;

private:

    /* State */

    /// Time elapsed in application since start
    /// Unlike real time, it only cumulates confirmed update delta times,
    /// not raw elapsed times, and is therefore a multiple of mp_frameDuration.
    sf::Time ms_applicationTime;
};
