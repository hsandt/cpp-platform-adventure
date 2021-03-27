#pragma once

// std
#include <Memory/Box.hpp>

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
class OwnerClass;
struct Transform;
class World;

/// ClassTemplate description
class ClassTemplate
{
public:

    /// Return the static value
    static int getStaticValue();

    explicit ClassTemplate(OwnerClass&);
    ~ClassTemplate();

    /// Update
    virtual void update(World& world, sf::Time deltaTime) = 0;

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

    const Box<Transform> mc_transform;


    /* Parameters */

    /// Initial health
    u16 mp_initialHealth;

protected:

    /* References to objects with guaranteed lifetime */

    /// Direct or indirect owner is guaranteed to live as long as this object,
    /// and when it dies, this object will die
    OwnerClass& mo_owner;

public:

    /* State */

    /// Current health
    u16 ms_initialHealth;
};

class ChildClass : public ClassTemplate
{
    ChildClass();
    ~ChildClass();

    /// Update
    virtual void update(World& world, sf::Time deltaTime) override;
};
