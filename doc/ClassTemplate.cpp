#include "ClassTemplate.h"

// std
#include <cassert>
#include <cmath>
#include <stdexcept>

// SFML
#include <SFML/Graphics.hpp>

// Game
#include "Space/World.h"

/* static */ int MyClass::s_staticValue = 1;

/* static */ int MyClass::getStaticValue()
{
    return s_staticValue;
}

MyClass::MyClass() :
    mp_initialHealth(100)
{
}

MyClass::~MyClass()
{
}

void MyClass::update(World& world, sf::Time elapsedTime)
{
    world.update(elapsedTime);
}

void MyClass::render(sf::RenderWindow& window)
{
}

void MyClass::incrementHealth()
{
    ++ms_initialHealth;
}
