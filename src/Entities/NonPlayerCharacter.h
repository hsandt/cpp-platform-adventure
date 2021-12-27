#pragma once

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Common.h"
#include "Entities/IInteractable.h"
#include "Space/SpatialObject.h"

// SFML
namespace sf
{
    class RectangleShape;
    class RenderTarget;
}

// yaml-cpp
namespace YAML
{
    class Node;
}

// Game
class DialogueTree;
class PlayerCharacter;
class World;
struct Transform;

class NonPlayerCharacter : public SpatialObject, public IInteractable
{
public:
    explicit NonPlayerCharacter(GameApplication& gameApp, Handle id);
    virtual ~NonPlayerCharacter();

    void deserialize(const YAML::Node& spatialObjectNode) override;

    /// Update
    virtual void update(World& world, sf::Time deltaTime) override;

    /// Render
    virtual void render(sf::RenderTarget& renderTarget) override;

    /// Callback for interaction
    virtual void onInteract(PlayerCharacter& playerCharacter) override;

public:

    /* Components */

    const Box<sf::RectangleShape> mc_shape;

    /* Parameters */

    /// Dialogue tree. Must be set manually after construction.
    const Box<DialogueTree> mp_dialogueTree;
};
