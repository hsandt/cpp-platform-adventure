#include "GameApplication.h"

#include <cassert>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "NonPlayerCharacter.h"
#include "PlayerCharacter.h"
#include "WindowConfig.h"

GameApplication::GameApplication() :
    window(std::make_unique<sf::RenderWindow>()),
    view(std::make_unique<sf::View>()),
    m_initialized(false),
    m_time()
{

}

// even if empty, destructor definition should be in .cpp
// so we don't need to #include T for each std::unique_ptr<T> members
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
    window->create(sf::VideoMode(windowConfig.width, windowConfig.height), windowConfig.title, sf::Style::Close, settings);

    // set framerate limit (not necessarily FPS, may be a little higher for extra precision)
    // doing this disables vsync
    window->setFramerateLimit(windowConfig.framerateLimit);

    // camera view
    view->setCenter({1280.f * 0.5f, 720.f * 0.5f});
    view->setSize({1280.f, 720.f});

    // confirm initialization
    m_initialized = true;
}

void GameApplication::run()
{
    assert(m_initialized);

    // grass rectangle (goes beyond the bottom of the screen on start to allow camera motion on Y)
    grass = std::make_unique<sf::RectangleShape>(sf::Vector2f{1280.f, 400.f});
    grass->setFillColor(sf::Color::Green);
    grass->setPosition(0.f, 420.f);

    // characters
    playerCharacter = std::make_unique<PlayerCharacter>();
    villager = std::make_unique<NonPlayerCharacter>();

    // time management
    sf::Clock clock;

    while (window->isOpen())
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
                window->close();
            }
        }

        update(elapsedTime);
        render();
    }
}

void GameApplication::update(sf::Time elapsedTime)
{
    // move camera
    view->move(0.f, std::sin(m_time.asSeconds()) * 50.f * elapsedTime.asSeconds());

    // update characters
    playerCharacter->update(elapsedTime);
    villager->update(elapsedTime);
}

void GameApplication::render()
{
    // clear sky
    window->clear(sf::Color::Cyan);

    // show grass with moving camera
    window->setView(*view);
    window->draw(*grass);

    // show characters
    playerCharacter->render(*window);
    villager->render(*window);

    // flip
    window->display();
}
