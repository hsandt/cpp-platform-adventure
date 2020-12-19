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

    /// Load scene content
    void loadScene();

    /// Update all world elements
    void update(sf::Time elapsedTime);

    /// Render all world elements
    void render(sf::RenderWindow& window);

    std::shared_ptr<NonPlayerCharacter>& getNonPlayerCharacter()
    {
        return nonPlayerCharacter;
    }

    std::unique_ptr<PlayerCharacter>& getPlayerCharacter()
    {
        return playerCharacter;
    }

private:

    /* Components */

    const std::unique_ptr<Terrain> terrain;
    std::unique_ptr<PlayerCharacter> playerCharacter;
    std::shared_ptr<NonPlayerCharacter> nonPlayerCharacter;
};
