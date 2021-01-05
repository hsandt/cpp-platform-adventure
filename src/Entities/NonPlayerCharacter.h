#pragma once

// std
#include <memory>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

// Game
#include "Entities/IInteractable.h"

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class World;
struct Transform;

class NonPlayerCharacter : public IInteractable
{
public:
    NonPlayerCharacter();
    ~NonPlayerCharacter();

public:
    void update(World& world, sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

    /// Callback for interaction
    virtual void onInteract() override;

public:

    /* Components */

    const std::unique_ptr<Transform> mc_transform;
    const std::unique_ptr<sf::RectangleShape> mc_shape;

    /* Parameters */

    std::string mp_dialogueText;
};
