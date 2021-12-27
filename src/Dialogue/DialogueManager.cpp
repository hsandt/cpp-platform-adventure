#include "Dialogue/DialogueManager.h"

// RmlUi
#include <RmlUi/Core.h>

// Game
#include "Application/GameApplication.h"
#include "Dialogue/DialogueTree.h"
#include "Entities/PlayerCharacter.h"
#include "Input/InputManager.h"
#include "Space/World.h"

DialogueManager::DialogueManager(GameApplication& gameApp) :
    ApplicationObject(gameApp),
    mr_dialogBox(),
    mr_dialogText()
{
}

DialogueManager::~DialogueManager()
{
}

void DialogueManager::init()
{
    SafePtr<Rml::Context> rmlContext = mo_gameApp.mr_rmlContext;
    PPK_ASSERT_DEBUG(rmlContext, "DialogueManager::init: No Rml Context");

    if (rmlContext)
    {
        // Load dialog box document
        mr_dialogBox = rmlContext->LoadDocument("assets/ui/dialog_box.rml");
        PPK_ASSERT_DEBUG(mr_dialogBox, "DialogueManager::init: Could not load document: dialog_box.rml");

        if (mr_dialogBox)
        {
            mr_dialogText = mr_dialogBox->GetElementById("text");
            PPK_ASSERT_DEBUG(mr_dialogText, "DialogueManager::init: No #text found on dialog box document");
        }
    }
}

void DialogueManager::handleInput()
{
    if (mr_dialogBox && mr_dialogBox->IsVisible() &&
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
    if (mr_dialogBox && mr_dialogText)
    {
        mr_dialogBox->Show();
        mr_dialogText->SetInnerRML(text);
    }
}

void DialogueManager::closeDialogue()
{
    if (mr_dialogBox)
    {
        mr_dialogBox->Hide();
    }

    mo_gameApp.mc_inputManager->popInputContext(InputContext::Dialogue);

    // allow player character to interact again
    auto oPlayerCharacter = mo_gameApp.mc_world->getPlayerCharacter();
    if (oPlayerCharacter)
    {
        oPlayerCharacter->setCanInteract(true);
    }
}

void DialogueManager::interact()
{
    // for now, only one dialogue text is allowed, so on first interaction,
    // close dialogue
    closeDialogue();
}
