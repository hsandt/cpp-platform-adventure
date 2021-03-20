#pragma once

// std
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>

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
    /// Path to scene assets directory
    static std::filesystem::path sceneAssetsDirPath;

    explicit World(GameApplication& gameApp);
    virtual ~World();

    /// Order World to load scene on next frame, by passing filepath relative to
    /// assets/scenes
    void deferLoadScene(const std::string& relativeFilePathString);

    /// Update all world elements
    void update(sf::Time deltaTime);

    /// Render all world elements
    void render(sf::RenderWindow& window);

    /// Return name of target scene of left gate, if any
    const std::optional<std::string>& GetGateLeftTargetSceneName() const
    {
        return ms_oGateLeftTargetSceneName;
    }

    /// Return name of target scene of right gate, if any
    const std::optional<std::string>& GetGateRightTargetSceneName() const
    {
        return ms_oGateRightTargetSceneName;
    }

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

    /// Load scene content from YAML scene asset file, described by filepath relative to
    /// assets/scenes
    void loadSceneFromYAML(const std::string& relativeFilePathString);

    /// Add spatial object from unique pointer
    SpatialObject& addSpatialObject(std::unique_ptr<SpatialObject> spatialObject);

    /// Destroy all spatial objects flagged for destruction
    void cleanObjectsToDestroy();

    /// Destroy all spatial objects
    void clearScene();

    /* Components */

    const std::unique_ptr<Terrain> terrain;


    /* State */

    /// File path of current scene
    std::optional<std::string> ms_oCurrentSceneFilePathString;

    /// File path of next scene to load, if any (consumed on next frame)
    std::optional<std::string> ms_oNextSceneFilePathString;

    /// Meta-object: left gate
    std::optional<std::string> ms_oGateLeftTargetSceneName;

    /// Meta-object: right gate
    std::optional<std::string> ms_oGateRightTargetSceneName;

    /// Map of spatial objects, identified by handle
    std::map<Handle, Box<SpatialObject>> ms_spatialObjects;

    /// Set of objects flagged for destruction at the end of the frame
    std::set<Handle> ms_spatialObjectHandlesFlaggedForDestruction;


    /* References to objects that may not always exist */

    /// Handle to player character
    SpatialObjectHandle ms_playerCharacterHandle;
};
