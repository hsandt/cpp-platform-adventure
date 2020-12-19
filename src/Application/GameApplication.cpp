#include "Application/GameApplication.h"

// std
#include <cassert>
#include <cmath>
#include <stdexcept>

// SFML
#include <SFML/Graphics.hpp>

// Game
#include "Dialogue/DialogueManager.h"
#include "Input/InputManager.h"
#include "UI/UIRoot.h"
#include "Application/WindowConfig.h"
#include "Space/World.h"

/* static */ GameApplication* GameApplication::singletonInstance = nullptr;

GameApplication::GameApplication() :
    m_initialized(false),
    m_time()
{
    // initialize as little as you can before setting singleton instance
    // to avoid risk of other objects trying to access this instance too early
    if (singletonInstance == nullptr)
    {
        singletonInstance = this;
    }
    else
    {
        throw(std::runtime_error("GameApplication singleton instance already registered, cannot register further constructed instances"));
    }
}

GameApplication::~GameApplication()
{
    // members will be destroyed automatically *after* this implementation
    // some of the world objects will unregister from GameApplication::get()
    // (which throws if singletonInstance is nullptr) in their own destructor,
    // so to be safe, clear the world now
    world.reset();

    singletonInstance = nullptr;
}

/* static */ GameApplication& GameApplication::get()
{
    if (singletonInstance == nullptr)
    {
        throw std::runtime_error("GameApplication singletonInstance not set, cannot get()");
    }

    return *singletonInstance;
}

void GameApplication::init()
{
    WindowConfig windowConfig = WindowConfig::from_file("config/window.yml");

    // set aliasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = windowConfig.antialiasingLevel;

    // create window
    window = std::make_unique<sf::RenderWindow>();

    // set window size (windowed, no resize)
    window->create(sf::VideoMode(windowConfig.width, windowConfig.height), windowConfig.title, sf::Style::Close, settings);

    // set framerate limit (not necessarily FPS, may be a little higher for extra precision)
    // doing this disables vsync
    window->setFramerateLimit(windowConfig.framerateLimit);

    // create camera view
    view = std::make_unique<sf::View>(
        sf::Vector2f(1280.f * 0.5f, 720.f * 0.5f),
        sf::Vector2f(1280.f, 720.f)
    );

    // create world
    world = std::make_unique<World>();

    // load initial scene
    world->loadScene();

    // confirm initialization
    m_initialized = true;
}

void GameApplication::run()
{
    assert(m_initialized);

    // time management
    sf::Clock clock;

    bool shouldRun = true;

    while (shouldRun)
    {
        // Time check
        sf::Time elapsedTime = clock.restart();
        m_time += elapsedTime;
        // anti-overflow (brutal)
        if (m_time.asSeconds() > 1000 * 1000)
        {
            m_time = sf::Time::Zero;
        }

        // Event handling
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                shouldRun = false;
                break;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    // apply action callback if any
                    if (m_OnSpacePressAction)
                    {
                        m_OnSpacePressAction();
                    }
                }
            }
        }

        if (shouldRun)
        {
            update(elapsedTime);
            render();
        }
    }

    window->close();
}

void GameApplication::assignSpacePressedAction(std::function<void()> action)
{
    // overwrite any previous action
    m_OnSpacePressAction = action;
}

void GameApplication::unassignSpacePressedAction()
{
    m_OnSpacePressAction = nullptr;
}

void GameApplication::update(sf::Time elapsedTime)
{
    // update input
    // TODO

    // move camera
    view->move(0.f, std::sin(m_time.asSeconds()) * 50.f * elapsedTime.asSeconds());

    // update characters
    world->update(elapsedTime);
}

void GameApplication::render()
{
    // clear sky
    window->clear(sf::Color::Cyan);

    // set view from moving camera
    window->setView(*view);

    // render world
    world->render(*window);

    // set view back to default view so UI has fixed position on screen
    window->setView(window->getDefaultView());

    // render UI
    uiRoot->render(*window);

    // flip
    window->display();
}
