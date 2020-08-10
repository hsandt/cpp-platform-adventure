#include "PlayerCharacter.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

PlayerCharacter::PlayerCharacter() :
    shape(std::make_unique<sf::RectangleShape>())
{
    // character rectangle
    shape->setSize(sf::Vector2f{32.f, 32.f});
    shape->setFillColor(sf::Color::Red);
    shape->setPosition(100.f, 400.f);
}

void PlayerCharacter::update(sf::Time elapsedTime)
{
    // Move character based on input

    // compute move intention on X
    float moveIntentionX = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        moveIntentionX -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        moveIntentionX += 1.f;
    }

    // apply character speed in px/s
    const float characterSpeedX = 32.f * 10;
    shape->move(characterSpeedX * elapsedTime.asSeconds() * moveIntentionX, 0.f);
}

void PlayerCharacter::render(sf::RenderWindow& window)
{
    window.draw(*shape);
}