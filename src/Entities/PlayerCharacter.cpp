#include "Entities/PlayerCharacter.h"

#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

// Game
#include "Application/GameApplication.h"
#include "Components/Transform.h"
#include "Entities/IInteractable.h"
#include "Entities/NonPlayerCharacter.h"
#include "Entities/PickUpItem.h"
#include "Input/InputManager.h"
#include "Memory/Box.hpp"
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

void PlayerCharacter::update(World& world, sf::Time deltaTime)
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
    mc_transform->position.x += characterSpeedX * deltaTime.asSeconds() * moveIntentionX;

    // Detect interactable elements around character
    detectInteractable(world);

    if (m_canInteract &&
        mo_gameApp.mc_inputManager->getCurrentInputContext() == InputContext::Platforming)
    {
        if (mo_gameApp.mc_inputManager->isKeyJustPressed(sf::Keyboard::Key::Space))
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
    // ms_detectedInteractable.reset();
    ms_detectedInteractable = 999;  // unfortunately potentially VALID!

    const std::map<Handle, Box<SpatialObject>>& spatialObjects = world.getSpatialObjects();
    for (const auto &[handle, spatialObject] : spatialObjects)
    {
        // check if it's an interactable item
        if ([[maybe_unused]] const IInteractable* interactable = dynamic_cast<const IInteractable*>(&*spatialObject))
        {
            // check if item center position is inside square centered
            // on player character with half-size (50, 50)
            sf::Vector2f vectorToItem = spatialObject->mc_transform->position - mc_transform->position;
            const float maxInteractDistance = 50.f;
            if (fabs(vectorToItem.x) < maxInteractDistance && fabs(vectorToItem.y) < maxInteractDistance)
            {
                // ms_detectedInteractable = std::move(SpatialObjectHandle(*mo_gameApp.mc_world, handle));
                ms_detectedInteractable = handle;
            }
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
        // if (std::shared_ptr<IInteractable> interactable = ms_detectedInteractable.lock())
        if (auto oInteractable = mo_gameApp.mc_world->findSpatialObject(ms_detectedInteractable))
        {
            // prevent further interaction, including input binding
            setCanInteract(false);

            ms_activeInteractable = ms_detectedInteractable;
            // ms_detectedInteractable.reset();
            ms_detectedInteractable = 999;
            // interactable->onInteract();
            if (IInteractable* interactable = dynamic_cast<IInteractable*>(&oInteractable->get()))
            {
                interactable->onInteract();
            }
        }
    }
}
