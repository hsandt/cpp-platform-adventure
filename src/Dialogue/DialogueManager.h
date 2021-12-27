#pragma once

// std
#include <string>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"

namespace Rml
{
    class Element;
    class ElementDocument;
}

// Game
class DialogueTree;

class DialogueManager : public ApplicationObject
{
public:
    explicit DialogueManager(GameApplication& gameApp);
    ~DialogueManager();

public:
    /// Initialize dialogue manager, loading required UI documents
    /// This must be called after GameApplication::initRmlUi
    void init();

    /// Process input
    void handleInput();

    /// Start dialogue tree by following nodes one by one from the Start node
    void startDialogueTree(const DialogueTree& dialogueTree);

    /// End dialogue and hide its UI
    void closeDialogue();

    /// Callback for interaction action
    void interact();

private:

    /// Show single dialogue text. This does not set Input Context
    /// and should be done as part of showDialogueTree
    void showDialogueText(const std::string& text);

private:

    /* External references */

    /// Dialog box
    SafePtr<Rml::ElementDocument> mr_dialogBox;

    /// Dialog text
    SafePtr<Rml::Element> mr_dialogText;
};
