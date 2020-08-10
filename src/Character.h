#pragma once

#include <memory>

#include <SFML/System/Time.hpp>

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class Character
{
public:
    Character();

public:
    void update(sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

private:
    /* Components */

    std::unique_ptr<sf::RectangleShape> shape;
};