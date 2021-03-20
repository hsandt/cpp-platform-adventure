#include "Entities/PlayerCharacter.h"

// std
#include <cmath>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

// fmt
#include "fmt/format.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Application/GameApplication.h"
#include "Components/Transform.h"
#include "Entities/IInteractable.h"
#include "Entities/NonPlayerCharacter.h"
#include "Entities/PickUpItem.h"
#include "Input/InputManager.h"
#include "Memory/Box.hpp"
#include "PlayerCharacter/Inventory.h"
#include "Serialization/YamlHelper.h"
#include "Space/World.h"

PlayerCharacter::PlayerCharacter(GameApplication& gameApp, Handle id) :
    SpatialObject(gameApp, id, /* persistentFlag: */ true),
    mc_transform(),
    mc_shape(),
    mc_inventory(),
    m_canInteract(false)  // so setCanInteract works
{
    // character rectangle
    mc_shape->setPosition(-16.f, -16.f);
    mc_shape->setSize(sf::Vector2f(32.f, 32.f));
    mc_shape->setFillColor(sf::Color::Red);

    setCanInteract(true);
}

PlayerCharacter::~PlayerCharacter()
{
    // important to remove input bindings
    setCanInteract(false);
}

/* static */ std::unique_ptr<SpatialObject> PlayerCharacter::deserialize(GameApplication& gameApp, const YAML::Node& spatialObjectNode)
{
    Handle id = YamlHelper::get<Handle>(spatialObjectNode, "id");
    auto playerCharacter = std::make_unique<PlayerCharacter>(gameApp, id);

    sf::Vector2 position = YamlHelper::asVector2f(spatialObjectNode["transform"]["position"]);
    playerCharacter->mc_transform->position = position;

    return playerCharacter;
}

void PlayerCharacter::update(World& world, sf::Time deltaTime)
{
    // Move character based on input

    // compute move intention on X
    float moveIntentionX = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        moveIntentionX -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        moveIntentionX += 1.f;
    }

    // apply character speed in px/s
    const float characterSpeedX = 32.f * 10;
    mc_transform->position.x += characterSpeedX * deltaTime.asSeconds() * moveIntentionX;

    // clamp character shape edges to scene edges
    if (mc_transform->position.x < 0.f)
    {
        if (const std::optional<std::string>& oGateLeftTargetSceneName = mo_gameApp.mc_world->GetGateLeftTargetSceneName())
        {
            // go to scene on the left
            std::string sceneFilePathString = fmt::format("{}.yml", *oGateLeftTargetSceneName);
            mo_gameApp.mc_world->deferLoadScene(sceneFilePathString);

            // warp character to the right of scene on the left
            mc_transform->position.x = 1280.f;
        }
        else
        {
            // nothing to the left, stop
            mc_transform->position.x = 0.f;
        }
    }
    else if (mc_transform->position.x > 1280.f)
    {
        if (const std::optional<std::string>& oGateRightTargetSceneName = mo_gameApp.mc_world->GetGateRightTargetSceneName())
        {
            // go to scene on the right
            std::string sceneFilePathString = fmt::format("{}.yml", *oGateRightTargetSceneName);
            mo_gameApp.mc_world->deferLoadScene(sceneFilePathString);

            // warp character to the left of scene on the right
            mc_transform->position.x = 0.f;
        }
        else
        {
            // nothing to the right, stop
            mc_transform->position.x = 1280.f;
        }
    }

    // Detect interactable elements around character
    detectInteractable(world);

    if (m_canInteract &&
        mo_gameApp.mc_inputManager->getCurrentInputContext() == InputContext::Platforming)
    {
        if (mo_gameApp.mc_inputManager->isKeyJustPressed(sf::Keyboard::Key::Space))
        {
            interact();
        }
    }
}

void PlayerCharacter::render(sf::RenderWindow& window)
{
    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    sfTransform.translate(mc_transform->position.x, mc_transform->position.y);
    window.draw(*mc_shape, sfTransform);
}

void PlayerCharacter::setCanInteract(bool value)
{
    if (m_canInteract != value)
    {
        m_canInteract = value;
    }
}

void PlayerCharacter::addToInventory(const PickUpItem& pickUpItem)
{
    mc_inventory->addItem(pickUpItem.dataID);
}

void PlayerCharacter::detectInteractable(World& world)
{
    ms_oDetectedInteractable.reset();

    const std::map<Handle, Box<SpatialObject>>& spatialObjects = world.getSpatialObjects();
    for (const auto &[handle, spatialObject] : spatialObjects)
    {
        if (!spatialObject->getDestructionFlag())
        {
            // check if it's an interactable item
            if ([[maybe_unused]] const IInteractable* interactable = dynamic_cast<const IInteractable*>(&*spatialObject))
            {
                // check if item center position is inside square centered
                // on player character with half-size (50, 50)
                sf::Vector2f vectorToItem = spatialObject->mc_transform->position - mc_transform->position;
                const float maxInteractDistance = 50.f;
                if (fabs(vectorToItem.x) < maxInteractDistance && fabs(vectorToItem.y) < maxInteractDistance)
                {
                    ms_oDetectedInteractable = std::make_optional(SpatialObjectHandle(*mo_gameApp.mc_world, handle));
                }
            }
        }
    }
}

void PlayerCharacter::interact()
{
    if (m_canInteract && ms_oDetectedInteractable)
    {
        std::optional<std::reference_wrapper<SpatialObject>> oDetectedInteractable = ms_oDetectedInteractable->findObject();
        // always interact with the interactable previously detected
        // (this is just to avoid doing an extra detection and match UI)
        if (oDetectedInteractable)
        {
            // prevent further interaction, including input binding
            setCanInteract(false);

            std::optional<SpatialObjectHandle> a = ms_oDetectedInteractable;
            ms_oActiveInteractable = ms_oDetectedInteractable;
            ms_oDetectedInteractable.reset();
            if (IInteractable* interactable = dynamic_cast<IInteractable*>(&oDetectedInteractable->get()))
            {
                interactable->onInteract(*this);
            }
        }
    }
}
