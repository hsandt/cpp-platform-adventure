#pragma once

#include <memory>

#include <SFML/System/Time.hpp>

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class NonPlayerCharacter
{
public:
    NonPlayerCharacter();

public:
    void update(sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

    void onInteract();

private:
    /* Components */

    std::unique_ptr<sf::RectangleShape> shape;
};
