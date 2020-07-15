#pragma once

#include <memory>

namespace sf
{
    class RenderWindow;
}

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
    /* State */

    /// Render window
    std::unique_ptr<sf::RenderWindow> window;
};