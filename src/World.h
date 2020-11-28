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
    void loadScene();
    void update(sf::Time elapsedTime);
    void render(sf::RenderWindow& window);

    std::shared_ptr<NonPlayerCharacter>& getNonPlayerCharacter()
    {
        return nonPlayerCharacter;
    }

private:

    /* Components */

    const std::unique_ptr<Terrain> terrain;
    std::unique_ptr<PlayerCharacter> playerCharacter;
    std::shared_ptr<NonPlayerCharacter> nonPlayerCharacter;
};
