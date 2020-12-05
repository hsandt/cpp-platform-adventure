#pragma once

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class Transform;
class World;

class NonPlayerCharacter
{
public:
    NonPlayerCharacter();
    ~NonPlayerCharacter();

public:
    void update(World& world, sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

    /// Callback for interaction
    void onInteract();

public:

    /* Components */

    const std::unique_ptr<Transform> mc_transform;
    const std::unique_ptr<sf::RectangleShape> mc_shape;
};
