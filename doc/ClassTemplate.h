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

/// ClassTemplate description
class ClassTemplate
{
public:
    /// Return the static value
    static int getStaticValue();

    ClassTemplate();
    ~ClassTemplate();

    /// Update
    virtual void update(World& world, sf::Time elapsedTime) = 0;

    /// Render
    virtual void render(sf::RenderWindow& window);

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

class ChildClass : public ClassTemplate
{
    ChildClass();
    ~ChildClass();

    /// Update
    virtual void update(World& world, sf::Time elapsedTime) override;
};
