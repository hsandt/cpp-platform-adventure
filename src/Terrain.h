#pragma once

#include <memory>

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class Terrain
{
public:
    Terrain();

public:
    void render(sf::RenderWindow& window);

private:
    /* Components */

    std::unique_ptr<sf::RectangleShape> shape;
};