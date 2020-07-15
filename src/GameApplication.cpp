#include "GameApplication.h"

#include <cmath>
#include <SFML/Graphics.hpp>

GameApplication::GameApplication()
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
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1280, 720), "Game", sf::Style::Close, settings);

    // 60 FPS, no vsync
    window->setFramerateLimit(60);
}

void GameApplication::run()
{
    // grass rectangle (goes beyond the bottom of the screen on start to allow camera motion on Y)
    sf::RectangleShape grass({1280.f, 400.f});
    grass.setFillColor(sf::Color::Green);
    grass.setPosition(0.f, 420.f);

    // camera view
    sf::View view;
    view.setCenter({1280.f * 0.5f, 720.f * 0.5f});
    view.setSize({1280.f, 720.f});

    // time management
    sf::Clock clock;
    sf::Time time;

    while (window->isOpen())
    {
        // Time check
        sf::Time elapsedTime = clock.restart();
        time += elapsedTime;
        if (time.asSeconds() > 1000 * 1000)
        {
            time = sf::Time::Zero;
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

        // move camera
        view.move(0.f, std::sin(time.asSeconds()) * 50.f * elapsedTime.asSeconds());

        // clear sky
        window->clear(sf::Color::Cyan);

        // show grass with moving camera
        window->setView(view);
        window->draw(grass);

        // flip
        window->display();
    }
}
