#pragma once

// std
#include <optional>
#include <string>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"

// Game
class DialogueTree;

class DialogueManager : protected ApplicationObject
{
public:
    explicit DialogueManager(GameApplication& gameApp);
    ~DialogueManager();

public:
    void handleInput();

    /// Start dialogue tree by following nodes one by one from the Start node
    void startDialogueTree(const DialogueTree& dialogueTree);

    void closeDialogue();

    /// Callback for interaction action
    void interact();

private:

    /// Show single dialogue text. This does not set Input Context
    /// and should be done as part of showDialogueTree
    void showDialogueText(const std::string& text);

public:

    /* State */

    /// Handle to current dialog box
    std::optional<Handle> ms_oDialogueBoxHandle;

    /// Handle to current dialog text
    std::optional<Handle> ms_oDialogueTextHandle;
};
