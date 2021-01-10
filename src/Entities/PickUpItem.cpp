#include "Entities/PickUpItem.h"

// std
#include <cassert>
#include <cmath>
#include <stdexcept>

// SFML
#include <SFML/Graphics.hpp>

// Game
#include "Application/GameApplication.h"
#include "Components/Transform.h"
#include "Dialogue/DialogueManager.h"
#include "Space/World.h"

PickUpItem::PickUpItem(GameApplication& gameApp) :
    ApplicationObject(gameApp)
{
    // item rectangle
    mc_shape->setPosition(0.f, 0.f);
    mc_shape->setSize(sf::Vector2f(10.f, 20.f));
    mc_shape->setFillColor(sf::Color::Yellow);
}

PickUpItem::~PickUpItem()
{
}

void PickUpItem::render(sf::RenderWindow& window)
{
    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    sfTransform.translate(mc_transform->position.x, mc_transform->position.y);
    window.draw(*mc_shape, sfTransform);
}

void PickUpItem::onInteract() /* override */
{
    mo_gameApp.mc_dialogueManager->showDialogueText(mp_pickUpText);
}
