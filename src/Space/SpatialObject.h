#pragma once

// std
#include <Memory/Box.hpp>

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"

// SFML
namespace sf
{
    class RenderWindow;
}

// Game
struct Transform;
class World;

/// Object located in World (always 2D)
class SpatialObject : protected ApplicationObject
{
public:

    SpatialObject(GameApplication& gameApp, Handle id);
    virtual ~SpatialObject();

    /// Is the object flagged for destruction?
    bool getDestructionFlag() const { return ms_destructionFlag; }

    /// Flag for destruction at the end of the frame
    void destroy();

    /// Update (empty implementation for sub-classes that don't need one)
    virtual void update(World& world, sf::Time deltaTime) {}

    /// Render (empty implementation for sub-classes that don't need one)
    virtual void render(sf::RenderWindow& window) {}

public:

    /* Components */

    const Box<Transform> mc_transform;


    /* Parameters */

    /// Unique spatial object ID
    const Handle mp_id;

private:

    /* State */

    bool ms_destructionFlag;
};
