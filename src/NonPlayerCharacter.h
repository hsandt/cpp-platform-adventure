#pragma once

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class World;

class NonPlayerCharacter
{
public:
    NonPlayerCharacter();

public:
    void update(World& world, sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

    // Quick way to get position before we switch to an engine-side
    // position, with shape just syncing to it
    sf::Vector2f getPosition() const { return shape->getPosition(); }

    /// Callback for interaction
    void onInteract();

private:
    /* Components */

    std::unique_ptr<sf::RectangleShape> shape;
};
