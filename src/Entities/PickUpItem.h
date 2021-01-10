#pragma once

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"
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
class PickUpItem : protected ApplicationObject, public IInteractable
{
public:
    PickUpItem(GameApplication& gameApp);
    virtual ~PickUpItem();

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
