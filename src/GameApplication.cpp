#include "GameApplication.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

GameApplication::GameApplication() :
    window(std::make_unique<sf::RenderWindow>()),
    m_initialized(false),
    m_time()
{

}

GameApplication::~GameApplication()
{
}

void GameApplication::init()
{
    // no aliasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    
    // windowed 720p. no resize
    window->create(sf::VideoMode(1280, 720), "Game", sf::Style::Close, settings);

    // 60 FPS, no vsync
    window->setFramerateLimit(60);

    // camera view
    view = std::make_unique<sf::View>();
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
}

void GameApplication::render()
{
    // clear sky
    window->clear(sf::Color::Cyan);

    // show grass with moving camera
    window->setView(*view);
    window->draw(*grass);

    // flip
    window->display();
}