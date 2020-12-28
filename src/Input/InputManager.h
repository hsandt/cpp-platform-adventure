#pragma once

// std
#include <map>
#include <stack>

// SFML
#include <SFML/Window/Event.hpp>     // sf::Event
#include <SFML/Window/Keyboard.hpp>  // sf::Keyboard::Key

// Game
#include "Common.h"
#include "Input/KeyDynamicState.h"  // KeyDynamicState
#include "Input/InputContext.h"     // InputContext

/// Handles game input state update and access
/// Devices supported: Keyboard
class InputManager
{
public:
    InputManager();
    ~InputManager();

    /// Process a key event. Other event types are ignored.
    void processEvent(sf::Event event);

    /// Update
    void update();

    /// Return InputContext on stack top
    InputContext getCurrentInputContext() const;

    /// Push a new InputContext onto the stack
    void pushInputContext(InputContext inputContext);

    /// Pop an InputContext from the stack top
    /// UB unless the stack is not empty and has the passed input context at the top
    void popInputContext(InputContext inputContext);

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

    /// Set key dynamic state directly to match KeyPressed/KeyReleased event.
    /// It always reset framesSinceLastStateChange to 0.
    /// If the key has no entry in the dynamic state map, a new one is created.
    void setKeyDynamicStateAfterEvent(sf::Keyboard::Key keyCode, bool isEventKeyPressed);

    /// Update key dynamic states based on static state this frame
    void updateInputStates();

    /// Return the new button state of a button based on its previous dynamic state and
    /// whether it is pressed this frame
    KeyDynamicState computeNewDynamicButtonState(KeyDynamicState oldDynamicState);

public:

    /* Static */

    /// Default key dynamic state, returned when no event has been received
    /// for a key since app start (it is not stored in the key dynamic state map, just returned)
    static constexpr KeyDynamicState defaultKeyDynamicState {
        .isPressed = false,
        // we set 255 frames as if key had been released for a long time, to avoid triggering
        // some action done when player has "just released" an input
        .framesSinceLastStateChange = 255
    };

private:

    /* State */

    /// Map of key -> dynamic state
    /// std::map is used as sparse array. We don't need ordering,
    /// but it's not too expensive for few entries, and useful for debug.
    std::map<sf::Keyboard::Key, KeyDynamicState> ms_keyDynamicStateMap;

    /// Stack of input contexts, the current one being at the top
    std::stack<InputContext> ms_inputContextStack;
};
