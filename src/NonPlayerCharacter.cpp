#include "NonPlayerCharacter.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

NonPlayerCharacter::NonPlayerCharacter() :
    shape(std::make_unique<sf::RectangleShape>())
{
    // character rectangle
    shape->setSize(sf::Vector2f{20.f, 38.f});
    shape->setFillColor(sf::Color::Blue);
    shape->setPosition(600.f, 400.f);
}

void NonPlayerCharacter::update(sf::Time elapsedTime)
{
    // Move character based on time

    // apply character speed in px/s
    const float characterSpeedX = 32.f * 10;
    shape->move(characterSpeedX * elapsedTime.asSeconds() * 0.5f, 0.f);
}

void NonPlayerCharacter::render(sf::RenderWindow& window)
{
    window.draw(*shape);
}