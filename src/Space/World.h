#pragma once

// std
#include <map>
#include <memory>

// SFML
#include <SFML/System/Time.hpp>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"
#include "Memory/Box.hpp"

namespace sf
{
    class RenderWindow;
}

class NonPlayerCharacter;
class PickUpItem;
class PlayerCharacter;
class Terrain;

class World : ApplicationObject
{
public:
    World(GameApplication& gameApp);
    virtual ~World();

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

    // we don't have Smart Handles to replace weak_ptr yet
    // and we are already storing ms_detectedInteractable and ms_activeInteractable
    // as std::weak_ptr<IInteractable> on character side anyway, so for now
    // we use shared_ptr instead of Box so we can convert to weak_ptr easily in
    // PlayerCharacter::detectInteractable
    // std::map<Handle, Box<PickUpItem>>& getPickUpItems()
    std::map<Handle, std::shared_ptr<PickUpItem>>& getPickUpItems()
    {
        return ms_pickUpItems;
    }

private:

    /* Components */

    const std::unique_ptr<Terrain> terrain;
    std::unique_ptr<PlayerCharacter> playerCharacter;
    std::shared_ptr<NonPlayerCharacter> nonPlayerCharacter;

    /// Map of owned pick up items, identified by handle
    // std::map<Handle, Box<PickUpItem>> ms_pickUpItems;
    std::map<Handle, std::shared_ptr<PickUpItem>> ms_pickUpItems;
};
