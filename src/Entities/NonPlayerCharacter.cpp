#include "Entities/NonPlayerCharacter.h"

// std
#include <iostream>

// SFML
#include <SFML/Graphics.hpp>

// Game
#include "Application/GameApplication.h"
#include "Components/Transform.h"
#include "Dialogue/DialogueManager.h"
#include "Dialogue/DialogueTree.h"

NonPlayerCharacter::NonPlayerCharacter(GameApplication& gameApp, Handle id) :
    SpatialObject(gameApp, id),
    mc_shape(),
    mp_dialogueTree(gameApp)
{
    // character rectangle
    mc_shape->setPosition(0.f, 0.f);
    mc_shape->setSize(sf::Vector2f(20.f, 38.f));
    mc_shape->setFillColor(sf::Color::Blue);
}

// Even empty, destructor definition in .cpp required if only class-forwarding
// some smart pointer contained types in .h
NonPlayerCharacter::~NonPlayerCharacter()
{
}

void NonPlayerCharacter::update(World& world, sf::Time deltaTime)
{
}

void NonPlayerCharacter::render(sf::RenderWindow& window)
{
    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    sfTransform.translate(mc_transform->position.x, mc_transform->position.y);
    window.draw(*mc_shape, sfTransform);
}

void NonPlayerCharacter::onInteract(PlayerCharacter& playerCharacter) /* override */
{
    mo_gameApp.mc_dialogueManager->startDialogueTree(*mp_dialogueTree);
}
