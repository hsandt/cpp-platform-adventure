#pragma once

#include <memory>

#include <SFML/System/Time.hpp>

namespace sf
{
    class RectangleShape;
    class RenderWindow;
    class View;
}

class Character;

/// Game Application. Handles game loop.
class GameApplication
{
public:
    GameApplication();
    virtual ~GameApplication();

    GameApplication(const GameApplication&) = delete;
    GameApplication& operator=(const GameApplication&) = delete;

public:
    /// Initialize game application.
    void init();

    /// Run game loop. Returns when the loop is over, i.e. the window is closed.
    void run();

private:
    void update(sf::Time elapsedTime);
    void render();

private:
    /* Components */

    /// Render window
    std::unique_ptr<sf::RenderWindow> window;

    /// View used to draw grass
    std::unique_ptr<sf::View> view;

    /// Grass to draw
    std::unique_ptr<sf::RectangleShape> grass;

    /// Character to draw
    std::unique_ptr<Character> character;

    /// NPC to draw
    std::unique_ptr<sf::RectangleShape> villager;

    /* State */

    /// Has the app been initialized?
    /// (currently used in debug only, consider #if DEBUG)
    bool m_initialized;

    /// Time elapsed since application start
    sf::Time m_time;
};
