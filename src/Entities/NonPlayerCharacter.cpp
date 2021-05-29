#include "Entities/NonPlayerCharacter.h"

// std
#include <iostream>

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
#include "Serialization/YamlConvert.hpp"
#include "Serialization/YamlHelper.hpp"

NonPlayerCharacter::NonPlayerCharacter(GameApplication& gameApp, Handle id) :
    SpatialObject(gameApp, id),
    mc_shape(),
    mp_dialogueTree(gameApp)
{
    // character rectangle
    mc_shape->setPosition(0.f, 0.f);
    mc_shape->setSize(sf::Vector2f(15.f, 16.f));
    mc_shape->setFillColor(sf::Color::Blue);
}

// Even empty, destructor definition in .cpp required if only class-forwarding
// some smart pointer contained types in .h
NonPlayerCharacter::~NonPlayerCharacter()
{
}

/* static */ std::unique_ptr<SpatialObject> NonPlayerCharacter::deserialize(GameApplication& gameApp, const YAML::Node& spatialObjectNode)
{
    Handle id = spatialObjectNode["id"].as<Handle>();
    auto nonPlayerCharacter = std::make_unique<NonPlayerCharacter>(gameApp, id);

    sf::Vector2 position = spatialObjectNode["transform"]["position"].as<sf::Vector2f>();
    nonPlayerCharacter->mc_transform->position = position;

    const YAML::Node& dialogueTreeNode = spatialObjectNode["dialogueTree"];

    // verifiedItemDataID is optional, but when there is one, it affects the dialogue condition checked
    // if not present, dialogueTextWithItem is ignored
    YamlHelper::tryGet<int>(dialogueTreeNode, "verifiedItemDataID", nonPlayerCharacter->mp_dialogueTree->mp_verifiedItemDataID);

    auto dialogueTextWithItem = dialogueTreeNode["dialogueTextWithItem"].as<std::string>();
    nonPlayerCharacter->mp_dialogueTree->mp_dialogueTextWithItem = dialogueTextWithItem;
    auto dialogueTextWithoutItem = dialogueTreeNode["dialogueTextWithoutItem"].as<std::string>();
    nonPlayerCharacter->mp_dialogueTree->mp_dialogueTextWithoutItem = dialogueTextWithoutItem;

    return nonPlayerCharacter;
}

void NonPlayerCharacter::update(World& world, sf::Time deltaTime)
{
}

void NonPlayerCharacter::render(sf::RenderTarget& renderTarget)
{
    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    sfTransform.translate(mc_transform->position.x, mc_transform->position.y);
    renderTarget.draw(*mc_shape, sfTransform);
}

void NonPlayerCharacter::onInteract(PlayerCharacter& playerCharacter) /* override */
{
    mo_gameApp.mc_dialogueManager->startDialogueTree(*mp_dialogueTree);
}
