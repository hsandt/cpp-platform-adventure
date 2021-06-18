#pragma once

// std
#include <map>
#include <optional>
#include <stack>

// SFML
#include <SFML/Window/Event.hpp>     // sf::Event
#include <SFML/Window/Keyboard.hpp>  // sf::Keyboard::Key

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"
#include "Input/KeyDynamicState.h"  // KeyDynamicState
#include "Input/InputContext.h"     // InputContext

/// Handles game input state update and access
/// Devices supported: Keyboard
class InputManager : public ApplicationObject
{
public:
    explicit InputManager(GameApplication& gameApp);
    ~InputManager();

    /// Process a key event. Other event types are ignored.
    /// This must always be called to update the key dynamic state map,
    /// and doesn't consume the event to let other objects handle it.
    void processEvent(sf::Event event);

    /// Update
    void update();

    /// Return current input context
    std::optional<InputContext> getCurrentInputContext() const;

    /// Push a new input context onto the stack
    void pushInputContext(InputContext inputContext);

    /// Pop an expected input context from the stack top
    /// UB unless the stack is not empty and has the passed input context at the top
    /// (passing the expected input context is only for confirmation, normal behavior
    /// doesn't change)
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

    /// Stack of input contexts. The one at the top becomes the current context
    /// at the beginning of the next frame, but not before.
    std::stack<InputContext> ms_inputContextStack;

    /// Input context to use this frame
    /// This is important to store so that even if the input context stack is modified
    /// during input handling, we don't change context in the middle of the frame,
    /// possibly causing unwanted reuse of same input in different contexts during one frame.
    /// It is only optional because there is no good default value as the enum doesn't contain None.
    /// You should push the first input context pretty early in the game, even if just to skip
    /// a splash screen.
    std::optional<InputContext> ms_oCurrentInputContext;
};
