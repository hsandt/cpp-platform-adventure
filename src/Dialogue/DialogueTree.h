#pragma once

// std
#include <string>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"

/// Dialogue tree
/// Allows to show sequence of text lines with branching
class DialogueTree : protected ApplicationObject
{
public:

    explicit DialogueTree(GameApplication& gameApp);
    virtual ~DialogueTree();

    /// Fo now, we use a very simplified version with only two dialogue lines based on
    /// whether item is in inventory
    const std::string& getTextFromContext() const;

public:

    /* Parameters */

    /// Text to show when player character has item in inventory
    std::string mp_dialogueTextWithItem;

    /// Text to show when player character doesn't have item in inventory
    std::string mp_dialogueTextWithoutItem;
};
