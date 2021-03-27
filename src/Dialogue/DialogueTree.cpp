#include "Dialogue/DialogueTree.h"

// Game
#include "Application/GameApplication.h"
#include "Data/ItemDataID.h"
#include "Entities/PlayerCharacter.h"
#include "PlayerCharacter/Inventory.h"
#include "Space/World.h"

const std::string DialogueTree::s_fallbackText = "NO PLAYER FOUND";

DialogueTree::DialogueTree(GameApplication& gameApp) :
    ApplicationObject(gameApp),
    mp_verifiedItemDataID(std::nullopt)
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
        if (mp_verifiedItemDataID && oPlayerCharacter->get().mc_inventory->hasItem(*mp_verifiedItemDataID))
        {
            return mp_dialogueTextWithItem;
        }
        else
        {
            return mp_dialogueTextWithoutItem;
        }
    }

    return s_fallbackText;
}
