#include "Entities/NonPlayerCharacter.h"

#include <iostream>

#include <SFML/System.hpp>

#include "Common.h"
#include "Components/Transform.h"
#include "UI/UIRoot.h"
#include "UI/UIWidgetRectangle.h"
#include "Application/GameApplication.h"

NonPlayerCharacter::NonPlayerCharacter() :
    mc_transform(std::make_unique<Transform>()),
    mc_shape(std::make_unique<sf::RectangleShape>())
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

void NonPlayerCharacter::update(World& world, sf::Time elapsedTime)
{
}

void NonPlayerCharacter::render(sf::RenderWindow& window)
{
    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    sfTransform.translate(mc_transform->position.x, mc_transform->position.y);
    window.draw(*mc_shape, sfTransform);
}

void NonPlayerCharacter::onInteract()
{
    auto dialogBox = std::make_unique<UIWidgetRectangle>();
    dialogBox->mc_transform->position = sf::Vector2f(100.f, 500.f);
    dialogBox->mc_shape->setPosition(0.f, 0.f);
    // dialogBox->mc_shape->setOrigin(sf::Vector2f(350.f, 50.f));
    dialogBox->mc_shape->setSize(sf::Vector2f(700.f, 100.f));
    dialogBox->mc_shape->setFillColor(sf::Color::Blue);
    Handle handle = GameApplication::get().getUIRoot()->addWidget(std::move(dialogBox));
    std::cout << "ON INTERACT" << std::endl;
}
