#pragma once

// std
#include <map>

// SFML
#include <SFML/Window/Keyboard.hpp>  // sf::Keyboard::Key

// Game
#include "Common.h"
#include "Input/KeyDynamicState.h"  // KeyDynamicState



/// Handles game input state update and access
/// Devices supported: Keyboard
class InputManager
{
public:
    InputManager();
    ~InputManager();

    /// Register a key for tracking
    void registerKey(sf::Keyboard::Key key);

    /// Update
    void update();

    /// Return true if the key has been pressed during this frame
    bool isKeyJustPressed(sf::Keyboard::Key key) const;

    /// Return true if the key has been released during this frame
    bool isKeyJustReleased(sf::Keyboard::Key key) const;

    /// Return true if the key is currently pressed
    bool isKeyPressed(sf::Keyboard::Key key) const;

    /// Return true if the key is currently released
    bool isKeyReleased(sf::Keyboard::Key key) const;

private:

    /// Return dynamic state of a key
    KeyDynamicState getKeyDynamicState(sf::Keyboard::Key key) const;

    /// Update key dynamic states based on static state this frame
    void processInputs();

    /// Return the new button state of a button based on its previous dynamic state and
    /// whether it is pressed this frame
    KeyDynamicState computeNewDynamicButtonState(KeyDynamicState oldDynamicState, bool isPressed);

private:

    /* State */

    /// Map of key -> dynamic state
    /// std::map is used as sparse array. We don't need ordering,
    /// but it's not too expensive for few entries, and useful for debug.
    std::map<sf::Keyboard::Key, KeyDynamicState> ms_keyDynamicStateMap;
};
