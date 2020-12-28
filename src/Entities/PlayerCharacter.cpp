#include "Entities/PlayerCharacter.h"

#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Application/GameApplication.h"
#include "Entities/NonPlayerCharacter.h"
#include "Input/InputManager.h"
#include "Space/World.h"
#include "Components/Transform.h"

PlayerCharacter::PlayerCharacter() :
    mc_transform(std::make_unique<Transform>()),
    mc_shape(std::make_unique<sf::RectangleShape>()),
    m_canInteract(false)  // so setCanInteract works
{
    // character rectangle
    mc_shape->setPosition(0.f, 0.f);
    mc_shape->setSize(sf::Vector2f(32.f, 32.f));
    mc_shape->setFillColor(sf::Color::Red);

    setCanInteract(true);

    // TODO: better in input or some manager
    GameApplication::get().mc_inputManager->registerKey(sf::Keyboard::Key::Space);
}

PlayerCharacter::~PlayerCharacter()
{
    // important to remove input bindings
    setCanInteract(false);
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
    mc_transform->position.x += characterSpeedX * elapsedTime.asSeconds() * moveIntentionX;

    // Detect interactable elements around character
    detectInteractable(world);

    if (m_canInteract &&
        GameApplication::get().mc_inputManager->getCurrentInputContext() == InputContext::Platforming)
    {
        if (GameApplication::get().mc_inputManager->isKeyJustPressed(sf::Keyboard::Key::Space))
        {
            interact();
        }
    }

}

void PlayerCharacter::render(sf::RenderWindow& window)
{
    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    sfTransform.translate(mc_transform->position.x, mc_transform->position.y);
    window.draw(*mc_shape, sfTransform);
}

void PlayerCharacter::detectInteractable(World& world)
{
    m_detectedInteractable.reset();

    std::shared_ptr<NonPlayerCharacter>& npc = world.getNonPlayerCharacter();
    if (npc)
    {
        // check if NPC center position is inside square centered
        // on player character with half-size (50, 50)
        sf::Vector2f vectorToNpc = npc->mc_transform->position - mc_transform->position;
        const float maxInteractDistance = 50.f;
        if (fabs(vectorToNpc.x) < maxInteractDistance && fabs(vectorToNpc.y) < maxInteractDistance)
        {
            m_detectedInteractable = npc;
        }
    }
}

void PlayerCharacter::setCanInteract(bool value)
{
    if (m_canInteract != value)
    {
        m_canInteract = value;

        // if (value)
        // {
        //     GameApplication::get().assignSpacePressedAction(std::bind(&PlayerCharacter::interact, this));
        // }
        // else
        // {
        //     GameApplication::get().unassignSpacePressedAction();
        // }
    }
}

void PlayerCharacter::interact()
{
    if (m_canInteract)
    {
        // always interact with the interactable previously detected
        // (this is just to avoid doing an extra detection and match UI)
        if (std::shared_ptr<NonPlayerCharacter> npc = m_detectedInteractable.lock())
        {
            // prevent further interaction, including input binding
            setCanInteract(false);

            m_activeInteractable = m_detectedInteractable;
            m_detectedInteractable.reset();
            npc->onInteract();
        }
    }
}
