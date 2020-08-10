#include "GameApplication.h"

#include <cassert>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "WindowConfig.h"
#include "World.h"

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

// even if empty, destructor definition should be in .cpp
// so we don't need to #include T for each std::unique_ptr<T> members
GameApplication::~GameApplication()
{
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
        }

        if (shouldRun)
        {
            update(elapsedTime);
            render();
        }
    }

    window->close();
}

void GameApplication::update(sf::Time elapsedTime)
{
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

    // flip
    window->display();
}
