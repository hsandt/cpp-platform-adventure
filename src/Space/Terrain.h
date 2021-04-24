#pragma once

#include <memory>

namespace sf
{
    class RectangleShape;
    class RenderTarget;
}

class Terrain
{
public:
    Terrain();

public:

    void render(sf::RenderTarget& renderTarget);

private:

    /* Components */

    const std::unique_ptr<sf::RectangleShape> shape;
};
