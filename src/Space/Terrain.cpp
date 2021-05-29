#include "Terrain.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

Terrain::Terrain() :
    shape(std::make_unique<sf::RectangleShape>())
{
    // grass rectangle goes beyond the bottom of the screen on start to allow camera motion on Y
    shape->setSize(sf::Vector2f{426.f, 400.f / 3.f});
    shape->setFillColor(sf::Color::Green);
    shape->setPosition(-426.f / 2.f, 20.f);
}

void Terrain::render(sf::RenderTarget& renderTarget)
{
    renderTarget.draw(*shape);
}
