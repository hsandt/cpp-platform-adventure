#pragma once

// Game
#include "Common.h"
#include "Entities/IInteractable.h"
#include "Memory/Box.hpp"

// SFML
namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

// Game
struct Transform;

/// PickUpItem description
class PickUpItem : public IInteractable
{
public:
    PickUpItem();
    ~PickUpItem();

    /// Render
    virtual void render(sf::RenderWindow& window);

    /// Callback for interaction
    virtual void onInteract() override;

public:

    /* Components */

    const Box<Transform> mc_transform;
    const Box<sf::RectangleShape> mc_shape;

    /* Parameters */

    /// Text displayed when picking this item. Must be set manually after construction.
    std::string mp_pickUpText;
};
