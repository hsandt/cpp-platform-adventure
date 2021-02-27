#pragma once

// Game
class PlayerCharacter;

/// Interface for entities that can be interacted with
struct IInteractable
{
    /// Handle interaction
    virtual void onInteract(PlayerCharacter& playerCharacter) = 0;
};
