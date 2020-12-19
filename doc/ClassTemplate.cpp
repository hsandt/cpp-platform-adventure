#include "Module/ClassTemplate.h"

// std
#include <cassert>
#include <cmath>
#include <stdexcept>

// SFML
#include <SFML/Graphics.hpp>

// Game
#include "Space/World.h"

/* static */ int ClassTemplate::s_staticValue = 1;

/* static */ int ClassTemplate::getStaticValue()
{
    return s_staticValue;
}

ClassTemplate::ClassTemplate() :
    mp_initialHealth(100)
{
}

ClassTemplate::~ClassTemplate()
{
}

void ClassTemplate::update(World& world, sf::Time elapsedTime)
{
    world.update(elapsedTime);
}

void ClassTemplate::render(sf::RenderWindow& window)
{
}

void ClassTemplate::incrementHealth()
{
    ++ms_initialHealth;
}
