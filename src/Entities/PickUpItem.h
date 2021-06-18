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
    class RenderTarget;
    class Sprite;
}

// yaml-cpp
namespace YAML
{
    class Node;
}

// Game
class DialogueTree;
class PlayerCharacter;
struct Transform;

/// PickUpItem description
class PickUpItem : public SpatialObject, public IInteractable
{
public:
    explicit PickUpItem(GameApplication& gameApp, Handle id);
    virtual ~PickUpItem();

    void deserialize(const YAML::Node& spatialObjectNode) override;

    /// Render
    virtual void render(sf::RenderTarget& renderTarget) override;

    /// Callback for interaction
    virtual void onInteract(PlayerCharacter& playerCharacter) override;

public:

    /* Components */

    const Box<sf::Sprite> mc_sprite;

    /* Parameters */

    /// Item Data ID
    DataID mp_dataID;

    /// Dialogue tree displayed when picking this item. Must be set manually after construction.
    const Box<DialogueTree> mp_pickUpDialogueTree;
};
