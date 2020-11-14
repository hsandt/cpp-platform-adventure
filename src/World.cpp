#include "World.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Terrain.h"
#include "NonPlayerCharacter.h"
#include "PlayerCharacter.h"

World::World()
{
}

World::~World()
{
}

void World::loadScene()
{
    // for now, we load a unique scene
    terrain = std::make_unique<Terrain>();
    playerCharacter = std::make_unique<PlayerCharacter>();
    nonPlayerCharacter = std::make_shared<NonPlayerCharacter>();
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
