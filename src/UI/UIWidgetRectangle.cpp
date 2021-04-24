#include "UI/UIWidgetRectangle.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Components/Transform.h"

UIWidgetRectangle::UIWidgetRectangle() :
    mc_transform(std::make_unique<Transform>()),
    mc_shape(std::make_unique<sf::RectangleShape>())
{
}

// Even empty, destructor definition in .cpp required if only class-forwarding
// some smart pointer contained types in .h
UIWidgetRectangle::~UIWidgetRectangle()
{
}

void UIWidgetRectangle::render(sf::RenderTarget& renderTarget)
{
    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    sfTransform.translate(mc_transform->position.x, mc_transform->position.y);
    renderTarget.draw(*mc_shape, sfTransform);
}
