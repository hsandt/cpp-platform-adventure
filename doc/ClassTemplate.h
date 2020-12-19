#pragma once

// std
#include <memory>

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Common.h"

// SFML
namespace sf
{
    class RenderWindow;
}

// Game
struct Transform;
class World;

/// MyClass description
class MyClass
{
public:
    /// Return the static value
    static int getStaticValue();

    MyClass();
    ~MyClass();

    /// Update
    void update(World& world, sf::Time elapsedTime);

    /// Render
    void render(sf::RenderWindow& window);

private:

    /// Increment health
    void incrementHealth();

public:
    /* Static */

    /// Static value
    static int s_staticValue;


    /* Components */

    const std::unique_ptr<Transform> mc_transform;


    /* Parameters */

    /// Initial health
    u16 mp_initialHealth;

private:
    /* State */

    /// Current health
    u16 ms_initialHealth;
};
