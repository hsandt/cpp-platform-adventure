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

ClassTemplate::ClassTemplate(OwnerClass& owner) :
    mo_owner(owner)
    mp_initialHealth(100)
{
}

ClassTemplate::~ClassTemplate()
{
}

void ClassTemplate::render(sf::RenderWindow& window)
{
}

void ClassTemplate::incrementHealth()
{
    ++ms_initialHealth;
}

ChildTemplate::ChildTemplate() :
    ClassTemplate()
{
}

ChildTemplate::~ChildTemplate()
{
}

void ChildTemplate::update(World& world, sf::Time elapsedTime) /* override */
{
    world.update(elapsedTime);
}
