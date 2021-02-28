#pragma once

// std
#include <map>
#include <memory>
#include <set>

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"
#include "Memory/Box.hpp"
#include "Space/SpatialObjectHandle.h"

namespace sf
{
    class RenderWindow;
}

class NonPlayerCharacter;
class PickUpItem;
class PlayerCharacter;
class SpatialObject;
class Terrain;

/// The world contains all spatial objects currently loaded in the game
class World : protected ApplicationObject
{
public:
    explicit World(GameApplication& gameApp);
    virtual ~World();

public:

    /// Load scene content
    void loadFirstScene();

    /// Load scene content from YAML scene asset
    void loadSceneFromYAML(const std::string& filename);

    /// Update all world elements
    void update(sf::Time deltaTime);

    /// Render all world elements
    void render(sf::RenderWindow& window);

    std::optional<std::reference_wrapper<PlayerCharacter>> getPlayerCharacter() const;

    const std::map<Handle, Box<SpatialObject>>& getSpatialObjects() const
    {
        return ms_spatialObjects;
    }

    std::optional<std::reference_wrapper<SpatialObject>> findSpatialObject(Handle handle) const;

    /// Flag spatial object for destruction at the end of the frame.
    /// If already flagged, does nothing. Does not check if handle is valid.
    /// Make sure to set destruction flag on object when calling this, so it stops
    /// receiving queries.
    void flagForDestruction(Handle handle);

private:

    /// Add spatial object from unique pointer
    SpatialObject& addSpatialObject(std::unique_ptr<SpatialObject> spatialObject);

    /// Destroy all spatial objects flagged for destruction
    void cleanObjectsToDestroy();

    /* Components */

    const std::unique_ptr<Terrain> terrain;


    /* State */

    /// Scene data: deserialized form of scene YAML asset
    // SceneData ms_currentSceneData;

    /// Map of spatial objects, identified by handle
    std::map<Handle, Box<SpatialObject>> ms_spatialObjects;

    /// Set of objects flagged for destruction at the end of the frame
    std::set<Handle> ms_spatialObjectHandlesFlaggedForDestruction;


    /* References to objects that may not always exist */

    /// Handle to player character
    SpatialObjectHandle ms_playerCharacterHandle;
};
