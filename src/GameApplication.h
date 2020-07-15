#pragma once

/// Game Application. Handles game loop.
class GameApplication
{
public:
    GameApplication();

    GameApplication(const GameApplication&) = delete;
    GameApplication& operator=(const GameApplication&) = delete;

public:
    void run();
};