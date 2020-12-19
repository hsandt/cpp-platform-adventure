#pragma once

// std
#include <functional>
#include <memory>

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
class UIRoot;
class World;

using onKeyPressFunc = std::function<void()>;

/// Game Application. Handles game loop.
class GameApplication
{
public:
    GameApplication();
    virtual ~GameApplication();

    GameApplication(const GameApplication&) = delete;
    GameApplication& operator=(const GameApplication&) = delete;

public:

    /// Return reference to singleton instance
    /// UB unless a GameApplication has been constructed
    /// which should always be the case unless calling during
    /// GameApplication construction or destruction itself
    static GameApplication& get();

    /// Initialize game application.
    void init();

    /// Run game loop. Returns when the loop is over, i.e. the window is closed.
    void run();

    const std::unique_ptr<World>& getWorld() const { return world; }
    const Box<UIRoot>& getUIRoot() const { return uiRoot; }
    const Box<DialogueManager>& getDialogueManager() const { return dialogueManager; }

    /// Register action for Space key Pressed
    void assignSpacePressedAction(onKeyPressFunc action);

    /// Unregister action for Space key Pressed
    void unassignSpacePressedAction();

private:
    /// Update the view and world
    void update(sf::Time elapsedTime);

    /// Render the world and HUD
    void render();

public:

    /* Components */

    /// Game world
    const Box<InputManager> mc_inputManager;

private:

    /* Singleton */

    /// Singleton instance
    static GameApplication* singletonInstance;


    /* Components */

    /// Render window
    std::unique_ptr<sf::RenderWindow> window;

    /// View used to draw grass
    std::unique_ptr<sf::View> view;

    /// Game world
    std::unique_ptr<World> world;

    /// Game UI
    const Box<UIRoot> uiRoot;

    /// Dialogue manager
    const Box<DialogueManager> dialogueManager;


    /* State */

    /// Has the app been initialized?
    /// (currently used in debug only, consider #if DEBUG)
    bool m_initialized;

    /// Time elapsed since application start
    sf::Time m_time;

    /// Callback associated to the Space key Press event
    onKeyPressFunc m_OnSpacePressAction;
};
