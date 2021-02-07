#pragma once

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Common.h"
#include "Entities/IInteractable.h"
#include "Memory/Box.hpp"
#include "Space/SpatialObject.h"

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class PlayerCharacter;
class World;
struct Transform;

class NonPlayerCharacter : public SpatialObject, public IInteractable
{
public:
    explicit NonPlayerCharacter(GameApplication& gameApp);
    virtual ~NonPlayerCharacter();

    /// Update
    virtual void update(World& world, sf::Time deltaTime) override;

    /// Render
    virtual void render(sf::RenderWindow& window) override;

    /// Callback for interaction
    virtual void onInteract(PlayerCharacter& playerCharacter) override;

public:

    /* Components */

    const Box<sf::RectangleShape> mc_shape;

    /* Parameters */

    /// Dialogue text. Must be set manually after construction.
    std::string mp_dialogueText;
};
