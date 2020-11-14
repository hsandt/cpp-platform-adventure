#include "NonPlayerCharacter.h"

#include <iostream>

#include <SFML/System.hpp>

NonPlayerCharacter::NonPlayerCharacter() :
    shape(std::make_unique<sf::RectangleShape>())
{
    // character rectangle
    shape->setSize(sf::Vector2f{20.f, 38.f});
    shape->setFillColor(sf::Color::Blue);
    shape->setPosition(600.f, 400.f);
}

void NonPlayerCharacter::update(World& world, sf::Time elapsedTime)
{
}

void NonPlayerCharacter::render(sf::RenderWindow& window)
{
    window.draw(*shape);
}

void NonPlayerCharacter::onInteract()
{
    std::cout << "ON INTERACT" << std::endl;
}
