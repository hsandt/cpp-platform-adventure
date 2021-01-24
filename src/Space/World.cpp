#include "Space/World.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Space/Terrain.h"
#include "Entities/NonPlayerCharacter.h"
#include "Entities/PickUpItem.h"
#include "Entities/PlayerCharacter.h"
#include "Components/Transform.h"

World::World(GameApplication& gameApp) :
    ApplicationObject(gameApp),
    terrain(std::make_unique<Terrain>())
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

    playerCharacter = std::make_unique<PlayerCharacter>(mo_gameApp);
    playerCharacter->mc_transform->position = sf::Vector2(550.f, 400.f);

    auto nonPlayerCharacter = std::make_unique<NonPlayerCharacter>(mo_gameApp);
    nonPlayerCharacter->mc_transform->position = sf::Vector2(600.f, 400.f);
    nonPlayerCharacter->mp_dialogueText = "Hello!";
    ms_spatialObjects.emplace(0, std::move(nonPlayerCharacter));

    auto item = std::make_unique<PickUpItem>(mo_gameApp);
    item->mc_transform->position = sf::Vector2(500.f, 400.f);
    item->mp_pickUpText = "Player picks item!";
    ms_spatialObjects.emplace(1, std::move(item));
}

void World::update(sf::Time deltaTime)
{
    // update characters
    playerCharacter->update(*this, deltaTime);

    // update spatial objects
    for (const auto &[handle, spatialObject] : ms_spatialObjects)
    {
        spatialObject->update(*this, deltaTime);
    }
}

void World::render(sf::RenderWindow& window)
{
    // show terrain
    terrain->render(window);

    // show characters
    playerCharacter->render(window);

    // show spatial objects
    for (const auto &[handle, spatialObject] : ms_spatialObjects)
    {
        spatialObject->render(window);
    }
}

std::optional<std::reference_wrapper<SpatialObject>> World::findSpatialObject(Handle handle) const
{
    auto it = ms_spatialObjects.find(handle);
    return it != ms_spatialObjects.end() ? std::optional{std::ref(*it->second)} : std::nullopt;
}
