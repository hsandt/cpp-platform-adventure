#include "Application/GameApplication.h"

// std
#include <cassert>
#include <cmath>
#include <stdexcept>

// SFML
#include <SFML/Graphics.hpp>

// Game
#include "Application/WindowConfig.h"
#include "Dialogue/DialogueManager.h"
#include "Input/InputManager.h"
#include "UI/UIRoot.h"
#include "Space/World.h"

GameApplication::GameApplication() :
    mc_dialogueManager(*this),
    mc_world(*this),
    m_initialized(false),
    m_time()
{
}

GameApplication::~GameApplication()
{
}

void GameApplication::init()
{
    WindowConfig windowConfig = WindowConfig::from_file("config/window.yml");

    // set aliasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = windowConfig.antialiasingLevel;

    // set window size (windowed, no resize)
    mc_window->create(sf::VideoMode(windowConfig.width, windowConfig.height), windowConfig.title, sf::Style::Close, settings);

    // set framerate limit (not necessarily FPS, may be a little higher for extra precision)
    // doing this disables vsync
    mc_window->setFramerateLimit(windowConfig.framerateLimit);

    // create camera view
    mc_view->setCenter(sf::Vector2f(1280.f * 0.5f, 720.f * 0.5f));
    mc_view->setSize(sf::Vector2f(1280.f, 720.f));

    // load initial scene
    mc_world->loadScene();

    // set initial input context to Platforming
    mc_inputManager->pushInputContext(InputContext::Platforming);

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
        while (mc_window->pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed ||
                (event.type == sf::Event::EventType::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
            {
                shouldRun = false;
                break;
            }
            else
            {
                mc_inputManager->processEvent(event);
            }
        }

        if (shouldRun)
        {
            update(elapsedTime);
            render();
        }
    }

    mc_window->close();
}

void GameApplication::update(sf::Time elapsedTime)
{
    // update input
    mc_inputManager->update();

    mc_dialogueManager->handleInput();

    // update characters
    mc_world->update(elapsedTime);

    // move camera
    mc_view->move(0.f, std::sin(m_time.asSeconds()) * 50.f * elapsedTime.asSeconds());
}

void GameApplication::render()
{
    // clear sky
    mc_window->clear(sf::Color::Cyan);

    // set view from moving camera
    mc_window->setView(*mc_view);

    // render world
    mc_world->render(*mc_window);

    // set view back to default view so UI has fixed position on screen
    mc_window->setView(mc_window->getDefaultView());

    // render UI
    mc_uiRoot->render(*mc_window);

    // flip
    mc_window->display();
}
