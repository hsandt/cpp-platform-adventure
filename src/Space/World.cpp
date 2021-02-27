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
            std::string type = YamlHelper::get<std::string>(spatialObjectNode, "type");
            Handle id = YamlHelper::get<Handle>(spatialObjectNode, "id");
            if (type == "PlayerCharacter")
            {
                auto playerCharacter = std::make_unique<PlayerCharacter>(mo_gameApp, id);

                sf::Vector2 position = YamlHelper::asVector2f(spatialObjectNode["transform"]["position"]);
                playerCharacter->mc_transform->position = position;

                addSpatialObject(std::move(playerCharacter));
            }
            else if (type == "NonPlayerCharacter")
            {
                auto nonPlayerCharacter = std::make_unique<NonPlayerCharacter>(mo_gameApp, id);

                sf::Vector2 position = YamlHelper::asVector2f(spatialObjectNode["transform"]["position"]);
                nonPlayerCharacter->mc_transform->position = position;

                const YAML::Node& dialogueTreeNode = spatialObjectNode["dialogueTree"];
                std::string dialogueTextWithItem = YamlHelper::get<std::string>(dialogueTreeNode, "dialogueTextWithItem");
                nonPlayerCharacter->mp_dialogueTree->mp_dialogueTextWithItem = dialogueTextWithItem;
                std::string dialogueTextWithoutItem = YamlHelper::get<std::string>(dialogueTreeNode, "dialogueTextWithoutItem");
                nonPlayerCharacter->mp_dialogueTree->mp_dialogueTextWithoutItem = dialogueTextWithoutItem;

                addSpatialObject(std::move(nonPlayerCharacter));
            }
            else if (type == "PickUpItem")
            {
                DataID dataID = YamlHelper::get<DataID>(spatialObjectNode, "dataID");
                auto item = std::make_unique<PickUpItem>(mo_gameApp, id, dataID);

                sf::Vector2 position = YamlHelper::asVector2f(spatialObjectNode["transform"]["position"]);
                item->mc_transform->position = position;

                const YAML::Node& shapeNode = spatialObjectNode["shape"];
                sf::Vector2 size = YamlHelper::asVector2f(shapeNode["size"]);
                item->mc_shape->setSize(size);
                sf::Color color = YamlHelper::asColor(shapeNode["fillColor"]);
                item->mc_shape->setFillColor(color);

                // currently dialogue trees all check for item, but it doesn't make sense for picking an item
                const YAML::Node& dialogueTree = spatialObjectNode["pickUpDialogueTree"];
                std::string dialogueTextWithItem = YamlHelper::get<std::string>(dialogueTree, "dialogueTextWithItem");
                item->mp_pickUpDialogueTree->mp_dialogueTextWithItem = dialogueTextWithItem;
                std::string dialogueTextWithoutItem = YamlHelper::get<std::string>(dialogueTree, "dialogueTextWithoutItem");
                item->mp_pickUpDialogueTree->mp_dialogueTextWithoutItem = dialogueTextWithoutItem;
                addSpatialObject(std::move(item));
            }
            else
            {
                throw std::runtime_error(fmt::format("Unsupported type {}", type));
            }

            // Check tag for special behaviours
            std::string tag;
            if (YamlHelper::tryGet<std::string>(spatialObjectNode, "tag", tag))
            {
                if (tag == "PlayerCharacter")
                {
                    // register as Player Character
                    ms_playerCharacterHandle.set(id);
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

void World::addSpatialObject(std::unique_ptr<SpatialObject> spatialObject)
{
    const auto& [it, success] = ms_spatialObjects.emplace(spatialObject->mp_id, std::move(spatialObject));

    if (!success)
    {
        // spatialObject was moved and lost, so get the ID from the conflicting existing object
        throw std::runtime_error(fmt::format(
            "World::addSpatialObject: there is already an object with id {}, "
            "cannot add another object with same id", it->second->mp_id));
    }
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
