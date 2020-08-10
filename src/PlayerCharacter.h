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

public:
    void update(sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

private:
    /* Components */

    std::unique_ptr<sf::RectangleShape> shape;
};