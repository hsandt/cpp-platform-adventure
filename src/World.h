#pragma once

#include <memory>

#include <SFML/System/Time.hpp>

namespace sf
{
    class RenderWindow;
}

class NonPlayerCharacter;
class PlayerCharacter;
class Terrain;

class World
{
public:
    World();
    ~World();

public:
    void update(sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

private:

    /* Components */

    std::unique_ptr<Terrain> terrain;
    std::unique_ptr<PlayerCharacter> playerCharacter;
    std::unique_ptr<NonPlayerCharacter> nonPlayerCharacter;
};
