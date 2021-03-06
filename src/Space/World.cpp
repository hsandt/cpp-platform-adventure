#include "Space/World.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Components/Transform.h"
#include "Data/ItemDataID.h"
#include "Dialogue/DialogueTree.h"
#include "Entities/NonPlayerCharacter.h"
#include "Entities/PickUpItem.h"
#include "Entities/PlayerCharacter.h"
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

void World::loadScene()
{
    // for now, we load a unique, hard-coded scene

    auto playerCharacter = std::make_unique<PlayerCharacter>(mo_gameApp, 0);
    playerCharacter->mc_transform->position = sf::Vector2(550.f, 400.f);
    bool success = addSpatialObject(std::move(playerCharacter));
    if (success)
    {
        ms_playerCharacterHandle.set(0);
    }

    auto nonPlayerCharacter = std::make_unique<NonPlayerCharacter>(mo_gameApp, 1);
    nonPlayerCharacter->mc_transform->position = sf::Vector2(600.f, 400.f);
    nonPlayerCharacter->mp_dialogueTree->mp_dialogueTextWithItem = "Wow, you brought me the flag. Thanks!";
    nonPlayerCharacter->mp_dialogueTree->mp_dialogueTextWithoutItem = "Hello! Can you bring me the flag over here?";
    addSpatialObject(std::move(nonPlayerCharacter));

    auto item = std::make_unique<PickUpItem>(mo_gameApp, 2, ItemDataID::Flag);
    item->mc_transform->position = sf::Vector2(500.f, 400.f);
    // currently dialogue trees all check for item, but it doesn't make sense for picking an item
    // but since we don't have item destruction/hiding yet, it's not a bad idea to still have some
    // feedback for having picked the item
    item->mc_shape->setSize(sf::Vector2(10.f, 20.f));
    item->mc_shape->setFillColor(sf::Color::Red);
    item->mp_pickUpDialogueTree->mp_dialogueTextWithItem = "Player has already picked flag!";
    item->mp_pickUpDialogueTree->mp_dialogueTextWithoutItem = "Player picks flag!";
    addSpatialObject(std::move(item));

    auto itemBox = std::make_unique<PickUpItem>(mo_gameApp, 3, ItemDataID::Box);
    itemBox->mc_transform->position = sf::Vector2(700.f, 400.f);
    itemBox->mc_shape->setSize(sf::Vector2(60.f, 30.f));
    itemBox->mc_shape->setFillColor(sf::Color::Yellow);
    itemBox->mp_pickUpDialogueTree->mp_dialogueTextWithItem = "Player has already picked box!";
    itemBox->mp_pickUpDialogueTree->mp_dialogueTextWithoutItem = "Player picks box!";
    addSpatialObject(std::move(itemBox));
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

bool World::addSpatialObject(std::unique_ptr<SpatialObject> spatialObject)
{
    const auto& [_it, success] = ms_spatialObjects.emplace(spatialObject->mp_id, std::move(spatialObject));
    return success;
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
