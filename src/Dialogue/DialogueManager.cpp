#include "Dialogue/DialogueManager.h"

// std
#include <memory>
#include <cassert>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Game
#include "Application/GameApplication.h"
#include "Components/Transform.h"
#include "Dialogue/DialogueTree.h"
#include "Entities/PlayerCharacter.h"
#include "Input/InputManager.h"
#include "Space/World.h"
#include "UI/UICanvas.h"
#include "UI/UIWidgetRectangle.h"
#include "UI/UIWidgetText.h"

DialogueManager::DialogueManager(GameApplication& gameApp) :
    ApplicationObject(gameApp),
    ms_oDialogueBoxHandle()
{
}

DialogueManager::~DialogueManager()
{
}

void DialogueManager::handleInput()
{
    if (ms_oDialogueBoxHandle &&
        mo_gameApp.mc_inputManager->getCurrentInputContext() == InputContext::Dialogue)
    {
        if (mo_gameApp.mc_inputManager->isKeyJustPressed(sf::Keyboard::Key::Space))
        {
            closeDialogue();
        }
    }
}

void DialogueManager::startDialogueTree(const DialogueTree& dialogueTree)
{
    mo_gameApp.mc_inputManager->pushInputContext(InputContext::Dialogue);

    showDialogueText(dialogueTree.getTextFromContext());
}

void DialogueManager::showDialogueText(const std::string& text)
{
    auto dialogBox = std::make_unique<UIWidgetRectangle>();
    dialogBox->mc_transform->position = sf::Vector2f(100.f, 500.f);
    dialogBox->mc_shape->setPosition(0.f, 0.f);
    // dialogBox->mc_shape->setOrigin(sf::Vector2f(350.f, 50.f));
    dialogBox->mc_shape->setSize(sf::Vector2f(700.f, 100.f));
    dialogBox->mc_shape->setFillColor(sf::Color::Blue);
    ms_oDialogueBoxHandle = mo_gameApp.mc_uiCanvas->addWidget(std::move(dialogBox));

    auto dialogText = std::make_unique<UIWidgetText>();
    dialogText->mc_transform->position = sf::Vector2f(150.f, 520.f);
    dialogText->mp_text = text;
    ms_oDialogueTextHandle = mo_gameApp.mc_uiCanvas->addWidget(std::move(dialogText));
}

void DialogueManager::closeDialogue()
{
    assert(ms_oDialogueBoxHandle);
    assert(ms_oDialogueTextHandle);

    mo_gameApp.mc_uiCanvas->removeWidget(ms_oDialogueBoxHandle);
    mo_gameApp.mc_uiCanvas->removeWidget(ms_oDialogueTextHandle);

    mo_gameApp.mc_inputManager->popInputContext(InputContext::Dialogue);

    // allow player character to interact again
    auto oPlayerCharacter = mo_gameApp.mc_world->getPlayerCharacter();
    if (oPlayerCharacter)
    {
        oPlayerCharacter->get().setCanInteract(true);
    }
}

void DialogueManager::interact()
{
    // for now, only one dialogue text is allowed, so on first interaction,
    // close dialogue
    closeDialogue();
}
