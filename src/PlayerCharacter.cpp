#include "PlayerCharacter.h"

#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "GameApplication.h"
#include "NonPlayerCharacter.h"
#include "World.h"
#include "Components/Transform.h"

PlayerCharacter::PlayerCharacter() :
    mc_transform(std::make_unique<Transform>()),
    mc_shape(std::make_unique<sf::RectangleShape>()),
    m_canInteract(true)
{
    // character rectangle
    mc_shape->setSize(sf::Vector2f{32.f, 32.f});
    mc_shape->setFillColor(sf::Color::Red);
    mc_shape->setPosition(100.f, 400.f);

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
    mc_shape->move(characterSpeedX * elapsedTime.asSeconds() * moveIntentionX, 0.f);

    // Detect interactable elements around character
    detectInteractable(world);
}

void PlayerCharacter::render(sf::RenderWindow& window)
{
    window.draw(*mc_shape);
}

void PlayerCharacter::detectInteractable(World& world)
{
    m_detectedInteractable.reset();

    std::shared_ptr<NonPlayerCharacter>& npc = world.getNonPlayerCharacter();
    if (npc)
    {
        // check if NPC center position is inside square centered
        // on player character with half-size (50, 50)
        sf::Vector2f vectorToNpc = npc->getPosition() - mc_shape->getPosition();
        const float maxInteractDistance = 50.f;
        if (fabs(vectorToNpc.x) < maxInteractDistance && fabs(vectorToNpc.y) < maxInteractDistance)
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
            m_activeInteractable = m_detectedInteractable;
            m_detectedInteractable.reset();
            npc->onInteract();
        }
    }
}
