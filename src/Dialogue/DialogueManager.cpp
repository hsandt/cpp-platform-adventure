#include "Dialogue/DialogueManager.h"

// std
#include <memory>
#include <cassert>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Game
#include "Common.h"
#include "Application/GameApplication.h"
#include "Components/Transform.h"
#include "Entities/PlayerCharacter.h"
#include "Space/World.h"
#include "UI/UIRoot.h"
#include "UI/UIWidgetRectangle.h"
#include "UI/UIWidgetText.h"

DialogueManager::DialogueManager() :
    ms_oDialogueBoxHandle()
{
}

DialogueManager::~DialogueManager()
{
}

void DialogueManager::showDialogueText(const std::string& text)
{
    auto dialogBox = std::make_unique<UIWidgetRectangle>();
    dialogBox->mc_transform->position = sf::Vector2f(100.f, 500.f);
    dialogBox->mc_shape->setPosition(0.f, 0.f);
    // dialogBox->mc_shape->setOrigin(sf::Vector2f(350.f, 50.f));
    dialogBox->mc_shape->setSize(sf::Vector2f(700.f, 100.f));
    dialogBox->mc_shape->setFillColor(sf::Color::Blue);
    ms_oDialogueBoxHandle = GameApplication::get().getUIRoot()->addWidget(std::move(dialogBox));

    auto dialogText = std::make_unique<UIWidgetText>();
    dialogText->mc_transform->position = sf::Vector2f(150.f, 520.f);
    dialogText->mp_text = text;
    ms_oDialogueTextHandle = GameApplication::get().getUIRoot()->addWidget(std::move(dialogText));

    GameApplication::get().assignSpacePressedAction(std::bind(&DialogueManager::interact, this));
}

void DialogueManager::closeDialogue()
{
    assert(ms_oDialogueBoxHandle);
    assert(ms_oDialogueTextHandle);

    GameApplication::get().getUIRoot()->removeWidget(*ms_oDialogueBoxHandle);
    GameApplication::get().getUIRoot()->removeWidget(*ms_oDialogueTextHandle);
    GameApplication::get().unassignSpacePressedAction();

    // allow player character to interact again
    // (only one binding is allowed at a time, so we need to unassignSpacePressedAction
    // before; although in this case, the line below will overwrite the action anyway)
    GameApplication::get().getWorld()->getPlayerCharacter()->setCanInteract(true);
}

void DialogueManager::interact()
{
    // for now, only one dialogue text is allowed, so on first interaction,
    // close dialogue
    closeDialogue();
}
