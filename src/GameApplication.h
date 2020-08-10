#pragma once

#include <memory>

#include <SFML/System/Time.hpp>

namespace sf
{
    class RenderWindow;
    class View;
}

class World;

/// Game Application. Handles game loop.
class GameApplication
{
public:
    GameApplication();
    virtual ~GameApplication();

    GameApplication(const GameApplication&) = delete;
    GameApplication& operator=(const GameApplication&) = delete;

public:

    /* Singleton */

    /// Return reference to singleton instance
    /// UB unless a GameApplication has been constructed
    /// which should be the case during the whole game
    static GameApplication& get() { return *singletonInstance; }


    /// Initialize game application.
    void init();

    /// Run game loop. Returns when the loop is over, i.e. the window is closed.
    void run();

private:
    void update(sf::Time elapsedTime);
    void render();

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


    /* State */

    /// Has the app been initialized?
    /// (currently used in debug only, consider #if DEBUG)
    bool m_initialized;

    /// Time elapsed since application start
    sf::Time m_time;
};
