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
#include "Dialogue/DialogueTree.h"
#include "Entities/PlayerCharacter.h"
#include "Space/World.h"

PickUpItem::PickUpItem(GameApplication& gameApp, Handle id, DataID dataID) :
    SpatialObject(gameApp, id),
    dataID(dataID),
    mp_pickUpDialogueTree(gameApp)
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

void PickUpItem::onInteract(PlayerCharacter& playerCharacter) /* override */
{
    mo_gameApp.mc_dialogueManager->startDialogueTree(*mp_pickUpDialogueTree);

    // destroy and add item to inventory after dialogue, in case dialogue checks inventory
    destroy();
    playerCharacter.addToInventory(*this);
}
