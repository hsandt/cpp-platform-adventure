#pragma once

// std
#include <map>
#include <memory>

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"
#include "Memory/Box.hpp"
#include "Space/SpatialObject.h"

namespace sf
{
    class RenderWindow;
}

class NonPlayerCharacter;
class PickUpItem;
class PlayerCharacter;
class Terrain;

/// The world contains all spatial objects currently loaded in the game
class World : protected ApplicationObject
{
public:
    explicit World(GameApplication& gameApp);
    virtual ~World();

public:

    /// Load scene content
    void loadScene();

    /// Update all world elements
    void update(sf::Time deltaTime);

    /// Render all world elements
    void render(sf::RenderWindow& window);

    std::shared_ptr<NonPlayerCharacter>& getNonPlayerCharacter()
    {
        return nonPlayerCharacter;
    }

    std::unique_ptr<PlayerCharacter>& getPlayerCharacter()
    {
        return playerCharacter;
    }

    const std::map<Handle, Box<SpatialObject>>& getSpatialObjects() const
    {
        return ms_spatialObjects;
    }

    std::optional<std::reference_wrapper<SpatialObject>> findSpatialObject(Handle handle) const;

private:

    /* Components */

    const std::unique_ptr<Terrain> terrain;
    std::unique_ptr<PlayerCharacter> playerCharacter;
    std::shared_ptr<NonPlayerCharacter> nonPlayerCharacter;

    /// Map of spatial objects, identified by handle
    std::map<Handle, Box<SpatialObject>> ms_spatialObjects;
};
