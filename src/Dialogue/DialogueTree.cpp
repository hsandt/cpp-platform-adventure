#include "Dialogue/DialogueTree.h"

// Game
#include "Application/GameApplication.h"
#include "Entities/PlayerCharacter.h"
#include "PlayerCharacter/Inventory.h"
#include "Space/World.h"

DialogueTree::DialogueTree(GameApplication& gameApp) :
    ApplicationObject(gameApp)
{
}

DialogueTree::~DialogueTree()
{
}

const std::string& DialogueTree::getTextFromContext() const
{
    auto oPlayerCharacter = mo_gameApp.mc_world->getPlayerCharacter();
    if (oPlayerCharacter)
    {
        // For now, we only check for a single item, the flag
        if (oPlayerCharacter->get().mc_inventory->hasFlag())
        {
            return mp_dialogueTextWithItem;
        }
        else
        {
            return mp_dialogueTextWithoutItem;
        }

    }
}
