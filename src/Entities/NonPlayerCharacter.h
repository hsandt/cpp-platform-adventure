#pragma once

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"
#include "Entities/IInteractable.h"
#include "Memory/Box.hpp"

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class World;
struct Transform;

class NonPlayerCharacter : protected ApplicationObject, public IInteractable
{
public:
    NonPlayerCharacter(GameApplication& gameApp);
    virtual ~NonPlayerCharacter();

    void update(World& world, sf::Time deltaTime);
    void render(sf::RenderWindow& window);

    /// Callback for interaction
    virtual void onInteract() override;

public:

    /* Components */

    const Box<Transform> mc_transform;
    const Box<sf::RectangleShape> mc_shape;

    /* Parameters */

    /// Dialogue text. Must be set manually after construction.
    std::string mp_dialogueText;
};
