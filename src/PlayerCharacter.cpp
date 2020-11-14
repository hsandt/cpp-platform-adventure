#include "PlayerCharacter.h"

#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "GameApplication.h"
#include "NonPlayerCharacter.h"
#include "World.h"

PlayerCharacter::PlayerCharacter() :
    shape(std::make_unique<sf::RectangleShape>()),
    m_canInteract(true)
{
    // character rectangle
    shape->setSize(sf::Vector2f{32.f, 32.f});
    shape->setFillColor(sf::Color::Red);
    shape->setPosition(100.f, 400.f);

    GameApplication::get().assignSpacePressedAction(std::bind(&PlayerCharacter::interact, this));
}

PlayerCharacter::~PlayerCharacter()
{
    GameApplication::get().unassignSpacePressedAction();
}

void PlayerCharacter::update(World& world, sf::Time elapsedTime)
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

    // Detect interactable elements around character
    detectInteractable(world);
}

void PlayerCharacter::render(sf::RenderWindow& window)
{
    window.draw(*shape);
}

void PlayerCharacter::detectInteractable(World& world)
{
    m_detectedInteractable.reset();

    std::shared_ptr<NonPlayerCharacter>& npc = world.getNonPlayerCharacter();
    if (npc)
    {
        // check if NPC center position is inside square centered
        // on player character with half-size (10, 10)
        sf::Vector2f vectorToNpc = npc->getPosition() - shape->getPosition();
        if (fabs(vectorToNpc.x) < 10.f && fabs(vectorToNpc.y) < 10.f)
        {
            m_detectedInteractable = npc;
        }
    }
}

void PlayerCharacter::interact()
{
    if (m_canInteract)
    {
        if (std::shared_ptr<NonPlayerCharacter> npc = m_detectedInteractable.lock())
        {
            npc->onInteract();
        }
    }
}
