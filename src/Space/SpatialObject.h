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
    class RenderTarget;
}

// Game
struct Transform;
class World;

/// Object located in World (always 2D)
class SpatialObject : protected ApplicationObject
{
public:

    SpatialObject(GameApplication& gameApp, Handle id, bool persistentFlag = false);
    virtual ~SpatialObject();

    /// Is the object flagged for destruction?
    bool getDestructionFlag() const { return ms_destructionFlag; }

    /// Flag for destruction at the end of the frame
    void destroy();

    /// Update (empty implementation for sub-classes that don't need one)
    virtual void update(World& world, sf::Time deltaTime) {}

    /// Render (empty implementation for sub-classes that don't need one)
    virtual void render(sf::RenderTarget& renderTarget) {}

public:

    /* Components */

    const Box<Transform> mc_transform;


    /* Parameters */

    /// Unique spatial object ID
    const Handle mp_id;

    /// Persistent flag: if true, the object is not destroyed on scene load
    const bool mp_persistentFlag;

private:

    /* State */

    /// Destruction flag: if true, the object will be destroyed at the end of the frame,
    /// and should not be retrieved by spatial object queries
    bool ms_destructionFlag;
};
