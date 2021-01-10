#pragma once

// std
#include <memory>

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Application/ApplicationObject.h"

// SFML
namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

// Game
class NonPlayerCharacter;
class World;
struct Transform;

struct IInteractable;

class PlayerCharacter : ApplicationObject
{
public:
    PlayerCharacter(GameApplication& gameApp);
    ~PlayerCharacter();

public:
    void update(World& world, sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

    /// Set whether this character can interact, and updating input bindings
    void setCanInteract(bool value);

private:
    /// Detect nearby interactable element
    void detectInteractable(World& world);

    /// Try to interact with a nearby element
    void interact();

public:

    /* Components */

    const std::unique_ptr<Transform> mc_transform;
    const std::unique_ptr<sf::RectangleShape> mc_shape;

private:

    /* State */

    /// Can the character interact now?
    bool m_canInteract;

    /// Interactable currently detected nearby (cleared on interaction start)
    std::weak_ptr<IInteractable> ms_detectedInteractable;

    /// Interactable currently interacted with (useful during interaction since ms_detectedInteractable is cleared)
    std::weak_ptr<IInteractable> ms_activeInteractable;
};
