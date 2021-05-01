#include "Input/InputManager.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Game
#include "Application/GameApplication.h"

InputManager::InputManager(GameApplication& gameApp) :
    ApplicationObject(gameApp)
{
}

InputManager::~InputManager()
{
}

void InputManager::processEvent(sf::Event event)
{
    if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::KeyReleased)
    {
        // we ignore any key modifiers (on Linux, combinations are not detected anyway)
        setKeyDynamicStateAfterEvent(event.key.code, event.type == sf::Event::EventType::KeyPressed);
    }
}

void InputManager::update()
{
    if (!ms_inputContextStack.empty())
    {
        // set the input context for this frame
        // even if the stack changes, it should not change until next frame for stability
        ms_oCurrentInputContext = ms_inputContextStack.top();
    }

    updateInputStates();
}

std::optional<InputContext> InputManager::getCurrentInputContext() const
{
    return ms_oCurrentInputContext;
}

void InputManager::pushInputContext(InputContext inputContext)
{
    ms_inputContextStack.emplace(inputContext);
}

void InputManager::popInputContext(InputContext inputContext)
{
    if (!ms_inputContextStack.empty())
    {
        InputContext top = ms_inputContextStack.top();
        if (top != inputContext)
        {
            throw std::runtime_error(fmt::format(
                "Input context stack top is {}, cannot pop expected input context {}.",
                top, inputContext
            ));
        }
    }
    else
    {
        throw std::runtime_error(fmt::format(
            "Input context stack is empty, cannot pop expected input context {}.",
            inputContext
        ));
    }

    ms_inputContextStack.pop();
}

bool InputManager::isKeyJustPressed(sf::Keyboard::Key key) const
{
	KeyDynamicState dynamicState = getKeyDynamicState(key);

    // if KeyPressed has been detected this frame and updateInputStates properly called,
    // framesSinceLastStateChange should now be 1, but we also check 0 just in case
	return dynamicState.isPressed && dynamicState.framesSinceLastStateChange <= 1;
}

bool InputManager::isKeyJustReleased(sf::Keyboard::Key key) const
{
	KeyDynamicState dynamicState = getKeyDynamicState(key);

    // see comment in isKeyJustPressed
	return !dynamicState.isPressed && dynamicState.framesSinceLastStateChange <= 1;
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) const
{
	KeyDynamicState dynamicState = getKeyDynamicState(key);
	return dynamicState.isPressed;
}

bool InputManager::isKeyReleased(sf::Keyboard::Key key) const
{
	KeyDynamicState dynamicState = getKeyDynamicState(key);
	return !dynamicState.isPressed;
}

KeyDynamicState InputManager::getKeyDynamicState(sf::Keyboard::Key key) const
{
    auto itKeyDynamicStatePair = ms_keyDynamicStateMap.find(key);

    if (itKeyDynamicStatePair == ms_keyDynamicStateMap.end())
    {
        // no entry found for this key, return default state (released for a long time)
        // we do not cache this default in the state map to avoid either dropping this method's
        // const or marking the map as mutable
        return defaultKeyDynamicState;
    }

    return itKeyDynamicStatePair->second;
}

void InputManager::setKeyDynamicStateAfterEvent(sf::Keyboard::Key keyCode, bool isEventKeyPressed)
{
    KeyDynamicState dynamicState = {
        .isPressed = isEventKeyPressed,
        // we start at 0 to remember we haven't processed this key in updateInputStates yet,
        // it will soon become 1 during this frame
        .framesSinceLastStateChange = 0
    };

    ms_keyDynamicStateMap[keyCode] = dynamicState;
}

void InputManager::updateInputStates()
{
    // setKeyDynamicStateAfterEvent already takes care of switching key state
    // so this method only increments frames since last state change (effectively moving
    // from a "just pressed/released" state to a "held pressed/released" state)
    for (auto& [key, dynamicState] : ms_keyDynamicStateMap)
    {
        // Auto-release all keys when window loses focus to avoid sticky keys
        // Note that we do not fix the reverse case: focusing window while holding key,
        // because keys not already in map would still be ignored. So we just ignore such keys
        // until they are actually pressed during window focus.
        if (!mo_gameApp.mc_window->hasFocus() && dynamicState.isPressed)
        {
            dynamicState.isPressed = false;
            dynamicState.framesSinceLastStateChange = 0;
        }

        // overflow check
        if (dynamicState.framesSinceLastStateChange < 255)
        {
            // increment frames count since press/release
            // if we have just detected a KeyPressed/KeyReleased event then the count
            // will become 1, which is still considered as "just pressed/released"
            ++dynamicState.framesSinceLastStateChange;
        }
    }
}
