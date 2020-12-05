#pragma once

// std
#include <optional>
#include <string>

// Game
#include "Common.h"

class DialogueManager
{
public:
    DialogueManager();
    ~DialogueManager();

public:
    void showDialogueText(const std::string& text);
    void closeDialogue();

    /// Callback for interaction action
    void interact();

public:

    /* State */

    /// Handle to current dialog box
    std::optional<Handle> ms_oDialogueBoxHandle;

    /// Handle to current dialog text
    std::optional<Handle> ms_oDialogueTextHandle;
};
