#include "Input/InputManager.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// SFML
#include <SFML/Window.hpp>

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::registerKey(sf::Keyboard::Key key)
{
	ms_keyDynamicStateMap.emplace(key, KeyDynamicState::HeldReleased);
}

void InputManager::update()
{
    processInputs();
}

InputContext InputManager::getCurrentInputContext() const
{
    return ms_inputContextStack.top();
}

void InputManager::pushInputContext(InputContext inputContext)
{
    ms_inputContextStack.emplace(inputContext);
}

void InputManager::popInputContext(InputContext inputContext)
{
    InputContext top = ms_inputContextStack.top();
    if (top != inputContext)
    {
        throw std::runtime_error(fmt::format(
            "Input context stack top is {}, cannot pop expected input context {}.",
            top, inputContext
        ));
    }

    ms_inputContextStack.pop();
}

bool InputManager::isKeyJustPressed(sf::Keyboard::Key key) const
{
	KeyDynamicState dynamicState = getKeyDynamicState(key);
	return dynamicState == KeyDynamicState::JustPressed;
}

bool InputManager::isKeyJustReleased(sf::Keyboard::Key key) const
{
	KeyDynamicState dynamicState = getKeyDynamicState(key);
	return dynamicState == KeyDynamicState::JustReleased;
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) const
{
	KeyDynamicState dynamicState = getKeyDynamicState(key);
	return dynamicState == KeyDynamicState::JustPressed || dynamicState == KeyDynamicState::HeldPressed;
}

bool InputManager::isKeyReleased(sf::Keyboard::Key key) const
{
	KeyDynamicState dynamicState = getKeyDynamicState(key);
	return dynamicState == KeyDynamicState::JustReleased || dynamicState == KeyDynamicState::HeldReleased;
}

KeyDynamicState InputManager::getKeyDynamicState(sf::Keyboard::Key key) const
{
    auto itKeyDynamicStatePair = ms_keyDynamicStateMap.find(key);

    if (itKeyDynamicStatePair == ms_keyDynamicStateMap.end())
    {
        throw std::runtime_error(fmt::format(
            "Key {} has not been registered to be tracked, InputManager cannot check isKeyJustPressed",
            key
        ));
    }

    return itKeyDynamicStatePair->second;
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
    if (oldDynamicState == KeyDynamicState::HeldReleased)
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
            return KeyDynamicState::HeldPressed;
        }
        else
        {
            return KeyDynamicState::JustReleased;
        }
    }
    else if (oldDynamicState == KeyDynamicState::HeldPressed)
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
            return KeyDynamicState::HeldReleased;
        }
    }

    // no change detected
    return oldDynamicState;
}
