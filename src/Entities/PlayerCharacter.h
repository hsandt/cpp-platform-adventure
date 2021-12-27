#pragma once

// std
#include <optional>

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Space/SpatialObject.h"
#include "Space/SpatialObjectHandle.h"

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
class Inventory;
class PickUpItem;
class World;
struct Transform;

struct IInteractable;

class PlayerCharacter : public SpatialObject
{
public:
    explicit PlayerCharacter(GameApplication& gameApp, Handle id);
    virtual ~PlayerCharacter();

    virtual void deserialize(const YAML::Node& spatialObjectNode) override;

    virtual void update(World& world, sf::Time deltaTime) override;
    virtual void render(sf::RenderTarget& renderTarget) override;

    /// Set whether this character can interact, and updating input bindings
    void setCanInteract(bool value);

    /// Add item to inventory
    void addToInventory(const PickUpItem& pickUpItem);

private:
    /// Detect nearby interactable element
    void detectInteractable(World& world);

    /// Try to interact with a nearby element
    void interact();

public:

    /* Components */

    const Box<Transform> mc_transform;
    const Box<sf::Sprite> mc_sprite;
    const Box<Inventory> mc_inventory;

private:

    /* State */

    /// Can the character interact now?
    bool ms_canInteract;

    /// Interactable currently detected nearby (cleared on interaction start)
    std::optional<SpatialObjectHandle> ms_oDetectedInteractable;

    /// Interactable currently interacted with (useful during interaction since ms_oDetectedInteractable is cleared)
    std::optional<SpatialObjectHandle> ms_oActiveInteractable;
};
