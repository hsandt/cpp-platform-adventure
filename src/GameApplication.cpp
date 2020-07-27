#include "GameApplication.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include "yaml-cpp/yaml.h"

#include "WindowConfig.h"

GameApplication::GameApplication() :
    window(std::make_unique<sf::RenderWindow>()),
    view(std::make_unique<sf::View>()),
    m_initialized(false),
    m_time()
{

}

GameApplication::~GameApplication()
{
}

void GameApplication::init()
{
    WindowConfig windowConfig;

    try
    {
        YAML::Node windowConfigFile = YAML::LoadFile("config/window.yml");

        // implicit conversion to unsigned int
        windowConfig.width = windowConfigFile["width"].as<int>();
        windowConfig.height = windowConfigFile["height"].as<int>();
        windowConfig.framerateLimit = windowConfigFile["framerateLimit"].as<int>();
        windowConfig.antialiasingLevel = windowConfigFile["antialiasingLevel"].as<int>();

        // implicit conversion to sf::String
        windowConfig.title = windowConfigFile["title"].as<std::string>();
    }
    catch(const YAML::BadFile& e)
    {
        std::cerr << e.what() << " config/window.yml, using default window config.\n";
    }

    // set aliasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = windowConfig.antialiasingLevel;

    // set window size (windowed, no resize)
    window->create(sf::VideoMode(windowConfig.width, windowConfig.height), windowConfig.title, sf::Style::Close, settings);

    // set framerate limit (not necessarily FPS, may be a little higher for extra precision)
    // doing this disables vsync
    std::cout << windowConfig.framerateLimit << std::endl;
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
