#include "Space/World.h"

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// fmt
#include "fmt/format.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Components/Transform.h"
#include "Data/ItemDataID.h"
#include "Dialogue/DialogueTree.h"
#include "Entities/NonPlayerCharacter.h"
#include "Entities/PickUpItem.h"
#include "Entities/PlayerCharacter.h"
#include "Serialization/Deserialization.h"
#include "Serialization/YamlHelper.h"
#include "Space/SpatialObject.h"
#include "Space/Terrain.h"

/* static */ std::filesystem::path World::sceneAssetsDirPath("assets/scenes");

World::World(GameApplication& gameApp) :
    ApplicationObject(gameApp),
    terrain(std::make_unique<Terrain>()),
    ms_oCurrentSceneFilePathString(),
    ms_oNextSceneFilePathString(),
    ms_oGateLeftTargetSceneName(),
    ms_oGateRightTargetSceneName(),
    ms_spatialObjects(),
    ms_spatialObjectHandlesFlaggedForDestruction(),
    ms_playerCharacterHandle(*this)
{
}

// Even empty, destructor definition in .cpp required if only class-forwarding
// some smart pointer contained types in .h
World::~World()
{
}

void World::deferLoadScene(const std::string& relativeFilePathString)
{
    ms_oNextSceneFilePathString = relativeFilePathString;
}

void World::update(sf::Time deltaTime)
{
    // Check for next scene to load
    // Note that next scene path can be equal to current scene, it will reload the current scene.
    if (ms_oNextSceneFilePathString)
    {
        loadSceneFromYAML(*ms_oNextSceneFilePathString);
        ms_oNextSceneFilePathString = std::nullopt;
    }

    // update spatial objects
    for (const auto &[handle, spatialObject] : ms_spatialObjects)
    {
        spatialObject->update(*this, deltaTime);
    }

    // Clean objects to destroy at the end of the update, so behavior updates can safely complete
    // Note that this is called multiple times if game application is catching up frames.
    cleanObjectsToDestroy();
}

void World::loadSceneFromYAML(const std::string& relativeFilePathString)
{
    // clear any loaded scene
    // this must only be called on game start and at the beginning of the frame,
    // so objects flagged for destruction have already been destroyed and we won't try 2x destruction
    clearScene();

    // clear meta-objects
    ms_oGateLeftTargetSceneName = std::nullopt;
    ms_oGateRightTargetSceneName = std::nullopt;

    // update current scene file path
    ms_oCurrentSceneFilePathString = relativeFilePathString;

    try
    {
        YAML::Node sceneAsset = YAML::LoadFile(sceneAssetsDirPath / relativeFilePathString);
        for (const YAML::Node& spatialObjectNode : sceneAsset)
        {
            // Check for meta-objects like Gates first
            // Redundant with Deserialization::deserialize...
            std::string type = YamlHelper::get<std::string>(spatialObjectNode, "type");
            if (type == "Gate")
            {
                std::string direction = YamlHelper::get<std::string>(spatialObjectNode, "direction");
                std::string targetScene = YamlHelper::get<std::string>(spatialObjectNode, "targetScene");
                if (direction == "left")
                {
                    ms_oGateLeftTargetSceneName = targetScene;
                }
                else if (direction == "right")
                {
                    ms_oGateRightTargetSceneName = targetScene;
                }

                // continue as meta-object is a valid entry, but deserialize below would assert on it
                continue;
            }

            std::unique_ptr<SpatialObject> spatialObject = Deserialization::deserialize(mo_gameApp, spatialObjectNode);

            // Check tag for special behaviours
            std::string tag;
            if (YamlHelper::tryGet<std::string>(spatialObjectNode, "tag", tag))
            {
                if (tag == "PlayerCharacter")
                {
                    if (ms_playerCharacterHandle)
                    {
                        // Player character has already been spawned and registered,
                        // so ignore this one (it's just the scene indicating a spawn point)
                        // Performance note: we created the object for nothing. It will be destroyed
                        // when unique_ptr goes out of scope, but it'd be better not to create it at all.
                        continue;
                    }

                    // Player Character found for the first time, register it and add it
                    ms_playerCharacterHandle.set(spatialObject->mp_id);
                }
            }

            addSpatialObject(std::move(spatialObject));
        }
    }
    catch(const YAML::BadFile& e)
    {
        // what() just contains "bad file", so prefer custom error message
        throw std::runtime_error(fmt::format("YAML::BadFile: '{}'", relativeFilePathString));
    }
}

SpatialObject& World::addSpatialObject(std::unique_ptr<SpatialObject> spatialObject)
{
    const auto& [it, success] = ms_spatialObjects.emplace(spatialObject->mp_id, std::move(spatialObject));

    if (!success)
    {
        // spatialObject was moved and lost, so get the ID from the conflicting existing object
        throw std::runtime_error(fmt::format(
            "World::addSpatialObject: there is already an object with id {}, "
            "cannot add another object with same id", it->second->mp_id));
    }

    return *it->second;
}

void World::cleanObjectsToDestroy()
{
    for (Handle handle : ms_spatialObjectHandlesFlaggedForDestruction)
    {
        // we assume handle is valid, but nothing happens if invalid besides returning 0
        ms_spatialObjects.erase(handle);
    }

    ms_spatialObjectHandlesFlaggedForDestruction.clear();
}

void World::clearScene()
{
    // save the persistent objects in a temporary map
    std::map<Handle, Box<SpatialObject>> persistentSpatialObjects;

    // don't increment iterator in for loop header because we are using the erase pattern
    // (except with extract)
    for (auto it = ms_spatialObjects.begin(); it != ms_spatialObjects.end();)
    {
        const auto &[handle, spatialObject] = *it;
        if (spatialObject->mp_persistentFlag)
        {
            persistentSpatialObjects.insert(ms_spatialObjects.extract(it++));
        }
        else
        {
            ++it;
        }
    }

    // replace current map of spatial objects with map only containing persistent objects,
    // effectively destroying all non-persistent objects
    ms_spatialObjects = std::move(persistentSpatialObjects);
}

void World::render(sf::RenderWindow& window)
{
    // show terrain
    terrain->render(window);

    // show spatial objects
    for (const auto &[handle, spatialObject] : ms_spatialObjects)
    {
        spatialObject->render(window);
    }
}

std::optional<std::reference_wrapper<PlayerCharacter>> World::getPlayerCharacter() const
{
    if (auto oPlayerCharacter = ms_playerCharacterHandle.findObject())
    {
        if (PlayerCharacter* pc = dynamic_cast<PlayerCharacter*>(&oPlayerCharacter->get()))
        {
            return *pc;
        }
    }

    return std::nullopt;
}

std::optional<std::reference_wrapper<SpatialObject>> World::findSpatialObject(Handle handle) const
{
    auto it = ms_spatialObjects.find(handle);
    return it != ms_spatialObjects.end() ? std::optional{std::ref(*it->second)} : std::nullopt;
}

void World::flagForDestruction(Handle handle)
{
    ms_spatialObjectHandlesFlaggedForDestruction.insert(handle);
}
