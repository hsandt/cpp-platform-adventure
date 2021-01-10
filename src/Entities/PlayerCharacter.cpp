#include "Entities/PlayerCharacter.h"

#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Application/GameApplication.h"
#include "Components/Transform.h"
#include "Entities/IInteractable.h"
#include "Entities/NonPlayerCharacter.h"
#include "Entities/PickUpItem.h"
#include "Input/InputManager.h"
#include "Space/World.h"

PlayerCharacter::PlayerCharacter(GameApplication& gameApp) :
    ApplicationObject(gameApp),
    mc_transform(std::make_unique<Transform>()),
    mc_shape(std::make_unique<sf::RectangleShape>()),
    m_canInteract(false)  // so setCanInteract works
{
    // character rectangle
    mc_shape->setPosition(0.f, 0.f);
    mc_shape->setSize(sf::Vector2f(32.f, 32.f));
    mc_shape->setFillColor(sf::Color::Red);

    setCanInteract(true);
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
    ms_detectedInteractable.reset();

    if (std::shared_ptr<NonPlayerCharacter>& npc = world.getNonPlayerCharacter())
    {
        // check if NPC center position is inside square centered
        // on player character with half-size (50, 50)
        sf::Vector2f vectorToNpc = npc->mc_transform->position - mc_transform->position;
        const float maxInteractDistance = 50.f;
        if (fabs(vectorToNpc.x) < maxInteractDistance && fabs(vectorToNpc.y) < maxInteractDistance)
        {
            ms_detectedInteractable = npc;
        }
    }

    // std::map<Handle, Box<PickUpItem>>& pickUpItems = world.getPickUpItems();
    std::map<Handle, std::shared_ptr<PickUpItem>>& pickUpItems = world.getPickUpItems();
    for (const auto &[handle, item] : pickUpItems)
    {
        // check if item center position is inside square centered
        // on player character with half-size (50, 50)
        sf::Vector2f vectorToItem = item->mc_transform->position - mc_transform->position;
        const float maxInteractDistance = 50.f;
        if (fabs(vectorToItem.x) < maxInteractDistance && fabs(vectorToItem.y) < maxInteractDistance)
        {
            ms_detectedInteractable = item;
        }
    }
}

void PlayerCharacter::setCanInteract(bool value)
{
    if (m_canInteract != value)
    {
        m_canInteract = value;
    }
}

void PlayerCharacter::interact()
{
    if (m_canInteract)
    {
        // always interact with the interactable previously detected
        // (this is just to avoid doing an extra detection and match UI)
        if (std::shared_ptr<IInteractable> interactable = ms_detectedInteractable.lock())
        {
            // prevent further interaction, including input binding
            setCanInteract(false);

            ms_activeInteractable = ms_detectedInteractable;
            ms_detectedInteractable.reset();
            interactable->onInteract();
        }
    }
}
