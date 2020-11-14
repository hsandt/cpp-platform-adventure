#pragma once

#include <memory>

#include <SFML/System/Time.hpp>

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class NonPlayerCharacter;
class World;

class PlayerCharacter
{
public:
    PlayerCharacter();
    ~PlayerCharacter();

public:
    void update(World& world, sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

private:
    /// Detect nearby interactable element
    void detectInteractable(World& world);

    /// Try to interact with a nearby element
    void interact();

private:

    /* Components */

    std::unique_ptr<sf::RectangleShape> shape;


    /* State */

    /// Can the character interact now?
    bool m_canInteract;

    /// Interactable NPC currently detected nearby (cleared on interaction start)
    std::weak_ptr<NonPlayerCharacter> m_detectedInteractable;

    /// NPC currently interacted with (useful during interaction since m_detectedInteractable is cleared)
    std::weak_ptr<NonPlayerCharacter> m_activeInteractable;
};
