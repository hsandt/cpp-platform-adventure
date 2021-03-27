#pragma once

// std
#include <optional>
#include <string>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"

/// Dialogue tree
/// Allows to show sequence of text lines with branching
/// Because this is currently the graph explorer and graph data at the same time,
/// we need to access game app so this is an ApplicationObject
/// Eventually this will be put in pure data and we will separate Dialogue Tree Data
/// and Dialogue Tree Explorer
class DialogueTree : protected ApplicationObject
{
public:

    /// Fallback text if player character is not found
    static const std::string s_fallbackText;

    explicit DialogueTree(GameApplication& gameApp);
    virtual ~DialogueTree();

    /// Fo now, we use a very simplified version with only two dialogue lines based on
    /// whether item is in inventory
    const std::string& getTextFromContext() const;

public:

    /* Parameters */

    /// Data ID of item verified to select text to show
    std::optional<DataID> mp_verifiedItemDataID;

    /// Text to show when player character has item in inventory
    std::string mp_dialogueTextWithItem;

    /// Text to show when player character doesn't have item in inventory
    std::string mp_dialogueTextWithoutItem;
};
