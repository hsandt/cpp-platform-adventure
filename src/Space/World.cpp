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

World::World(GameApplication& gameApp) :
    ApplicationObject(gameApp),
    terrain(std::make_unique<Terrain>()),
    ms_playerCharacterHandle(*this)
{
}

// Even empty, destructor definition in .cpp required if only class-forwarding
// some smart pointer contained types in .h
World::~World()
{
}

void World::loadFirstScene()
{
    // load as much as you can from asset file
    loadSceneFromYAML("assets/scenes/scene1.yml");
}

void World::loadSceneFromYAML(const std::string& filename)
{
    try
    {
        YAML::Node sceneAsset = YAML::LoadFile(filename);
        for (const YAML::Node& spatialObjectNode : sceneAsset)
        {
            SpatialObject& spatialObject = addSpatialObject(Deserialization::deserialize(mo_gameApp, spatialObjectNode));

            // Check tag for special behaviours
            std::string tag;
            if (YamlHelper::tryGet<std::string>(spatialObjectNode, "tag", tag))
            {
                if (tag == "PlayerCharacter")
                {
                    // register as Player Character
                    ms_playerCharacterHandle.set(spatialObject.mp_id);
                }
            }
        }
    }
    catch(const YAML::BadFile& e)
    {
        // what() just contains "bad file", so prefer custom error message
        throw std::runtime_error(fmt::format("YAML::BadFile: '{}'", filename));
    }
}

void World::update(sf::Time deltaTime)
{
    // update spatial objects
    for (const auto &[handle, spatialObject] : ms_spatialObjects)
    {
        spatialObject->update(*this, deltaTime);
    }

    // clean objects to destroy at the end of the update, so behavior updates can safely complete
    // note that this is called multiple times if game application is catching up frames
    cleanObjectsToDestroy();
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
