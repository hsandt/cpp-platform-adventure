#include "Entities/PickUpItem.h"

// std
#include <cassert>
#include <cmath>
#include <stdexcept>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Application/GameApplication.h"
#include "Components/Transform.h"
#include "Dialogue/DialogueManager.h"
#include "Dialogue/DialogueTree.h"
#include "Entities/PlayerCharacter.h"
#include "Graphics/TextureManager.h"
#include "Serialization/YamlConvert.hpp"
#include "Serialization/YamlHelper.hpp"
#include "Space/World.h"

PickUpItem::PickUpItem(GameApplication& gameApp, Handle id) :
    SpatialObject(gameApp, id),
    mp_dataID(0),  // maybe better if 0 was INVALID_ID
    mp_pickUpDialogueTree(gameApp)
{
    // sprite pivot (hardcoded for now, but should be in data)
    mc_sprite->setPosition(-5.f * 1.f, -10.f * 1.f);

    // default texture scaling is not smooth (nearest-neighbor), so just scale up
    // rendering is still following screen resolution, so either round sprite rendering position
    // to nearest scaled pixel, or work on a small texture then upscale it to become the window texture
    mc_sprite->setScale(1.f, 1.f);
}

PickUpItem::~PickUpItem()
{
}

void PickUpItem::deserialize(const YAML::Node& spatialObjectNode)
{
    mp_dataID = spatialObjectNode["dataID"].as<DataID>();

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

    // currently dialogue trees all check for item, but it doesn't make sense for picking an item
    const YAML::Node& dialogueTree = spatialObjectNode["pickUpDialogueTree"];
    auto dialogueTextWithItem = dialogueTree["dialogueTextWithItem"].as<std::string>();
    mp_pickUpDialogueTree->mp_dialogueTextWithItem = dialogueTextWithItem;
    auto dialogueTextWithoutItem = dialogueTree["dialogueTextWithoutItem"].as<std::string>();
    mp_pickUpDialogueTree->mp_dialogueTextWithoutItem = dialogueTextWithoutItem;
}

void PickUpItem::render(sf::RenderTarget& renderTarget)
{
    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    // round to nearest scaled integer pixel
    float roundedX = std::roundf(mc_transform->position.x / 1.f) * 1.f;
    float roundedY = std::roundf(mc_transform->position.y / 1.f) * 1.f;
    sfTransform.translate(roundedX, roundedY);
    renderTarget.draw(*mc_sprite, sfTransform);
}

void PickUpItem::onInteract(PlayerCharacter& playerCharacter) /* override */
{
    mo_gameApp.mc_dialogueManager->startDialogueTree(*mp_pickUpDialogueTree);

    // destroy and add item to inventory after dialogue, in case dialogue checks inventory
    destroy();
    playerCharacter.addToInventory(*this);
}
