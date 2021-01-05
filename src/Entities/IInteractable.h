#pragma once

/// Interface for entities that can be interacted with
struct IInteractable
{
    /// Handle interaction
    virtual void onInteract() = 0;
};
