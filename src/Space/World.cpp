#include "Space/World.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Space/Terrain.h"
#include "Entities/NonPlayerCharacter.h"
#include "Entities/PlayerCharacter.h"
#include "Components/Transform.h"

World::World() :
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

    playerCharacter = std::make_unique<PlayerCharacter>();
    playerCharacter->mc_transform->position = sf::Vector2(550.f, 400.f);

    nonPlayerCharacter = std::make_shared<NonPlayerCharacter>();
    nonPlayerCharacter->mc_transform->position = sf::Vector2(600.f, 400.f);
}

void World::update(sf::Time elapsedTime)
{
    // update characters
    playerCharacter->update(*this, elapsedTime);
    nonPlayerCharacter->update(*this, elapsedTime);
}

void World::render(sf::RenderWindow& window)
{
    // show terrain
    terrain->render(window);

    // show characters
    playerCharacter->render(window);
    nonPlayerCharacter->render(window);
}
