#include "NonPlayerCharacter.h"

#include <iostream>

#include <SFML/System.hpp>

#include "Components/Transform.h"

NonPlayerCharacter::NonPlayerCharacter() :
    mc_transform(std::make_unique<Transform>()),
    mc_shape(std::make_unique<sf::RectangleShape>())
{
    // character rectangle
    mc_shape->setSize(sf::Vector2f{20.f, 38.f});
    mc_shape->setFillColor(sf::Color::Blue);
    mc_shape->setPosition(600.f, 400.f);
}

// Even empty, destructor definition in .cpp required if only class-forwarding
// some smart pointer contained types in .h
NonPlayerCharacter::~NonPlayerCharacter()
{
}

void NonPlayerCharacter::update(World& world, sf::Time elapsedTime)
{
}

void NonPlayerCharacter::render(sf::RenderWindow& window)
{
    window.draw(*mc_shape);
}

void NonPlayerCharacter::onInteract()
{
    std::cout << "ON INTERACT" << std::endl;
}
