#include "TitleMenuState.h"

// RmlUi
#include <RmlUi/Core.h>

// Game
#include "Application/GameApplication.h"
#include "Input/InputManager.h"

TitleMenuState::TitleMenuState(GameApplication& gameApp) :
    GameState(gameApp)
{
}

TitleMenuState::~TitleMenuState()
{
}

void TitleMenuState::onEnter()
{
    SafePtr<Rml::Context> rmlContext = mo_gameApp.mr_rmlContext;
    PPK_ASSERT_DEBUG(rmlContext, "TitleMenuState::onEnter: No Rml Context");

    if (rmlContext)
    {
        // Load title menu document
        mr_titleMenuDocument = rmlContext->LoadDocument("assets/ui/title_menu.rml");
        PPK_ASSERT_DEBUG(mr_titleMenuDocument, "DialogueManager::init: Could not load document: title_menu.rml");

        if (mr_titleMenuDocument)
        {
            mr_titleMenuDocument->Show();
        //     mr_dialogText = mr_titleMenuDocument->GetElementById("text");
        //     PPK_ASSERT_DEBUG(mr_dialogText, "DialogueManager::init: No #text found on dialog box document");
        }
    }

    // set initial input context to Menu
    mo_gameApp.mc_inputManager->pushInputContext(InputContext::Menu);
}

void TitleMenuState::update()
{
}

void TitleMenuState::onExit()
{
    if (mr_titleMenuDocument)
    {
        // Close (unload) title menu document and clear pointer
        // as object will be destroyed on next Context::Update()
        mr_titleMenuDocument->Close();
        mr_titleMenuDocument = nullptr;
    }
}

