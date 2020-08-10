#pragma once

#include <memory>

#include <SFML/System/Time.hpp>

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class PlayerCharacter
{
public:
    PlayerCharacter();
    ~PlayerCharacter();

public:
    void update(sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

private:
    /// Try to interact with a nearby element
    void interact();

private:

    /* Components */

    std::unique_ptr<sf::RectangleShape> shape;
};
