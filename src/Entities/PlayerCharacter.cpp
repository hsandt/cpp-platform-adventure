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
#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"
#include "Memory/Box.hpp"
#include "PlayerCharacter/Inventory.h"
#include "Serialization/YamlConvert.hpp"
#include "Serialization/YamlHelper.hpp"
#include "Space/World.h"

PlayerCharacter::PlayerCharacter(GameApplication& gameApp, Handle id) :
    SpatialObject(gameApp, id, /* persistentFlag: */ true),
    mc_transform(),
    mc_sprite(),
    mc_inventory(),
    ms_canInteract(false)  // so setCanInteract works
{
    // sprite pivot, scaled (hardcoded for now, but should be in data)
    mc_sprite->setPosition(-8.f, -16.f);

    // default texture scaling is not smooth (nearest-neighbor), so just scale up
    // rendering is still following screen resolution, so either round sprite rendering position
    // to nearest scaled pixel, or work on a small texture then upscale it to become the window texture
    // mc_sprite->setScale(8.f, 8.f);

    setCanInteract(true);
}

PlayerCharacter::~PlayerCharacter()
{
    // important to remove input bindings
    setCanInteract(false);
}

void PlayerCharacter::deserialize(const YAML::Node& spatialObjectNode)
{
    sf::Vector2 position = spatialObjectNode["transform"]["position"].as<sf::Vector2f>();
    mc_transform->position = position;

    const YAML::Node& spriteNode = spatialObjectNode["sprite"];
    auto spriteTextureRelativePathString = spriteNode["texture"].as<std::string>();
    const sf::Texture& texture = mo_gameApp.mc_textureManager->loadFromFile(spriteTextureRelativePathString);
    mc_sprite->setTexture(texture);

    sf::IntRect sourceRect;
    if (YamlHelper::tryGet<sf::IntRect>(spriteNode, "rectangle", sourceRect))
    {
        mc_sprite->setTextureRect(sourceRect);
    }
}

void PlayerCharacter::update(World& world, sf::Time deltaTime)
{
    // Move character based on input

    // compute move intention on X
    float moveIntentionX = 0.f;
    if (mo_gameApp.mc_inputManager->isKeyPressed(sf::Keyboard::Left))
    {
        moveIntentionX -= 1.f;
    }
    if (mo_gameApp.mc_inputManager->isKeyPressed(sf::Keyboard::Right))
    {
        moveIntentionX += 1.f;
    }

    // apply character speed in px/s
    const float characterSpeedX = 106.f;
    mc_transform->position.x += characterSpeedX * deltaTime.asSeconds() * moveIntentionX;

    if (mc_transform->position.x < -213.f)
    {
        if (const std::optional<std::string>& oGateLeftTargetSceneName = mo_gameApp.mc_world->GetGateLeftTargetSceneName())
        {
            // go to scene on the left
            mo_gameApp.mc_world->deferLoadScene(*oGateLeftTargetSceneName);

            // warp character to the right of scene on the left
            mc_transform->position.x = 213.f;
        }
        else
        {
            // nothing to the left, stop
            mc_transform->position.x = -213.f;
        }
    }
    else if (mc_transform->position.x > 213.f)
    {
        if (const std::optional<std::string>& oGateRightTargetSceneName = mo_gameApp.mc_world->GetGateRightTargetSceneName())
        {
            // go to scene on the right
            mo_gameApp.mc_world->deferLoadScene(*oGateRightTargetSceneName);

            // warp character to the left of scene on the right
            mc_transform->position.x = -213.f;
        }
        else
        {
            // nothing to the right, stop
            mc_transform->position.x = 213.f;
        }
    }

    // Detect interactable elements around character
    detectInteractable(world);

    if (ms_canInteract &&
        mo_gameApp.mc_inputManager->getCurrentInputContext() == InputContext::Platforming)
    {
        if (mo_gameApp.mc_inputManager->isKeyJustPressed(sf::Keyboard::Key::Space))
        {
            interact();
        }
    }
}

void PlayerCharacter::render(sf::RenderTarget& renderTarget)
{
    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    // round to nearest scaled integer pixel
    float roundedX = std::roundf(mc_transform->position.x / 1.f);
    float roundedY = std::roundf(mc_transform->position.y / 1.f);
    sfTransform.translate(roundedX, roundedY);
    renderTarget.draw(*mc_sprite, sfTransform);
}

void PlayerCharacter::setCanInteract(bool value)
{
    if (ms_canInteract != value)
    {
        ms_canInteract = value;
    }
}

void PlayerCharacter::addToInventory(const PickUpItem& pickUpItem)
{
    mc_inventory->addItem(pickUpItem.mp_dataID);
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
                const float maxInteractDistance = 16.f;
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
    if (ms_canInteract && ms_oDetectedInteractable)
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
