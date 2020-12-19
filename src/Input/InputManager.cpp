#include "Input/InputManager.h"

// SFML
#include <SFML/Window.hpp>

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::update()
{
    processInputs();
}

void InputManager::processInputs()
{
    // SFML only provides a function to get the static state of a key (pressed or released).
    // In order to track the dynamic state of keys (released, just pressed, pressed, just released),
    // we need to monitor the evolution of their static states.
    for (auto& [key, dynamicState] : ms_keyDynamicStateMap)
    {
        dynamicState = computeNewDynamicButtonState(dynamicState, sf::Keyboard::isKeyPressed(key));
    }
}

KeyDynamicState InputManager::computeNewDynamicButtonState(KeyDynamicState oldDynamicState, bool isPressed)
{
    if (oldDynamicState == KeyDynamicState::Released)
    {
        if (isPressed)
        {
            return KeyDynamicState::JustPressed;
        }
    }
    else if (oldDynamicState == KeyDynamicState::JustPressed)
    {
        if (isPressed)
        {
            return KeyDynamicState::Pressed;
        }
        else
        {
            return KeyDynamicState::JustReleased;
        }
    }
    else if (oldDynamicState == KeyDynamicState::Pressed)
    {
        if (!isPressed)
        {
            return KeyDynamicState::JustReleased;
        }
    }
    else  // (oldDynamicState == KeyDynamicState::JustReleased)
    {
        if (isPressed)
        {
            return KeyDynamicState::JustPressed;
        }
        else
        {
            return KeyDynamicState::Released;
        }
    }

    // no change detected
    return oldDynamicState;
}
