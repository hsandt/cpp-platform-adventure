#pragma once

// Game
#include "Common.h"
#include "Entities/IInteractable.h"
#include "Memory/Box.hpp"
#include "Space/SpatialObject.h"

// SFML
namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

// Game
class DialogueTree;
class PlayerCharacter;
struct Transform;

/// PickUpItem description
class PickUpItem : public SpatialObject, public IInteractable
{
public:
    explicit PickUpItem(GameApplication& gameApp, Handle id, DataID dataID);
    virtual ~PickUpItem();

    /// Render
    virtual void render(sf::RenderWindow& window) override;

    /// Callback for interaction
    virtual void onInteract(PlayerCharacter& playerCharacter) override;

public:

    /* Components */

    const Box<sf::RectangleShape> mc_shape;

    /* Parameters */

    /// Item Data ID
    const DataID dataID;

    /// Dialogue tree displayed when picking this item. Must be set manually after construction.
    const Box<DialogueTree> mp_pickUpDialogueTree;
};
