#include "Entities/PickUpItem.h"

// std
#include <cassert>
#include <cmath>
#include <stdexcept>

// SFML
#include <SFML/Graphics.hpp>

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Application/GameApplication.h"
#include "Components/Transform.h"
#include "Dialogue/DialogueManager.h"
#include "Dialogue/DialogueTree.h"
#include "Entities/PlayerCharacter.h"
#include "Serialization/YamlHelper.h"
#include "Space/World.h"

PickUpItem::PickUpItem(GameApplication& gameApp, Handle id, DataID dataID) :
    SpatialObject(gameApp, id),
    dataID(dataID),
    mp_pickUpDialogueTree(gameApp)
{
    // item rectangle
    mc_shape->setPosition(0.f, 0.f);
    mc_shape->setSize(sf::Vector2f(10.f, 20.f));
    mc_shape->setFillColor(sf::Color::Yellow);
}

PickUpItem::~PickUpItem()
{
}

/* static */ std::unique_ptr<SpatialObject> PickUpItem::deserialize(GameApplication& gameApp, const YAML::Node& spatialObjectNode)
{
    Handle id = YamlHelper::get<Handle>(spatialObjectNode, "id");
    DataID dataID = YamlHelper::get<DataID>(spatialObjectNode, "dataID");
    auto item = std::make_unique<PickUpItem>(gameApp, id, dataID);

    sf::Vector2 position = YamlHelper::asVector2f(spatialObjectNode["transform"]["position"]);
    item->mc_transform->position = position;

    const YAML::Node& shapeNode = spatialObjectNode["shape"];
    sf::Vector2 size = YamlHelper::asVector2f(shapeNode["size"]);
    item->mc_shape->setSize(size);
    sf::Color color = YamlHelper::asColor(shapeNode["fillColor"]);
    item->mc_shape->setFillColor(color);

    // currently dialogue trees all check for item, but it doesn't make sense for picking an item
    const YAML::Node& dialogueTree = spatialObjectNode["pickUpDialogueTree"];
    std::string dialogueTextWithItem = YamlHelper::get<std::string>(dialogueTree, "dialogueTextWithItem");
    item->mp_pickUpDialogueTree->mp_dialogueTextWithItem = dialogueTextWithItem;
    std::string dialogueTextWithoutItem = YamlHelper::get<std::string>(dialogueTree, "dialogueTextWithoutItem");
    item->mp_pickUpDialogueTree->mp_dialogueTextWithoutItem = dialogueTextWithoutItem;

    return item;
}

void PickUpItem::render(sf::RenderWindow& window)
{
    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    sfTransform.translate(mc_transform->position.x, mc_transform->position.y);
    window.draw(*mc_shape, sfTransform);
}

void PickUpItem::onInteract(PlayerCharacter& playerCharacter) /* override */
{
    mo_gameApp.mc_dialogueManager->startDialogueTree(*mp_pickUpDialogueTree);

    // destroy and add item to inventory after dialogue, in case dialogue checks inventory
    destroy();
    playerCharacter.addToInventory(*this);
}
