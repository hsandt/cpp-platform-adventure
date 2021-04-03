#include "Application/GameApplication.h"

// std
#include <cmath>

// SFML
#include <SFML/Graphics.hpp>

// Game
#include "Application/AppConfig.h"
#include "Application/WindowConfig.h"
#include "Audio/MusicManager.h"
#include "Dialogue/DialogueManager.h"
#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"
#include "UI/UICanvas.h"
#include "Space/World.h"

GameApplication::GameApplication() :
    mc_world(*this),
    mc_uiCanvas(*this),
    mc_inputManager(*this),
    mc_textureManager(*this),
    mc_musicManager(*this),
    mc_dialogueManager(*this)
{
}

GameApplication::~GameApplication()
{
}

void GameApplication::init_and_run()
{
    init();
    run();
}

void GameApplication::init()
{
    AppConfig appConfig = AppConfig::from_file("config/app.yml");
    mp_frameDuration = sf::seconds(1.f / appConfig.fps);
    mp_maxUpdatesPerRender = appConfig.maxUpdatesPerRender;

    WindowConfig windowConfig = WindowConfig::from_file("config/window.yml");

    // set aliasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = windowConfig.antialiasingLevel;

    // set window size (windowed, no resize)
    mc_window->create(sf::VideoMode(windowConfig.width, windowConfig.height), windowConfig.title, sf::Style::Close, settings);

    // disable key repeat (this is not part of WindowConfig because most games either don't use it
    // or implement their own repeat detection system)
    mc_window->setKeyRepeatEnabled(false);

    // center window on screen by getting current desktop resolution and placing top-left accordingly
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    mc_window->setPosition(sf::Vector2i((desktopMode.width - windowConfig.width) / 2, (desktopMode.height - windowConfig.height) / 2));

    if (windowConfig.vsync)
    {
        // enable V-sync
        mc_window->setVerticalSyncEnabled(true);
    }

    // SFML only supports either V-sync of framerate limit, not both.
    // However, an experimental option allows to combine both since we noticed it's equivalent to
    // sleep until next frame after every render, which could also just be done in the update loop.
    if (!windowConfig.vsync || windowConfig.allowVsyncWithFramerateLimit)
    {
        // set framerate limit (not necessarily FPS, may be a little higher for extra precision)
        mc_window->setFramerateLimit(windowConfig.framerateLimit);
    }

    // create camera view
    mc_view->setCenter(sf::Vector2f(1280.f * 0.5f, 720.f * 0.5f));
    mc_view->setSize(sf::Vector2f(1280.f, 720.f));

    // load initial scene
    mc_world->deferLoadScene("scene1.yml");

    // load initial BGM
    mc_musicManager->playBgm("bgm1.ogg");

    // set initial input context to Platforming
    mc_inputManager->pushInputContext(InputContext::Platforming);
}

void GameApplication::run()
{
    // time management
    sf::Clock clock;
    sf::Time cumulatedElapsedTime;

    bool shouldRun = true;

    while (shouldRun)
    {
        // Time check
        sf::Time elapsedTime = clock.restart();
        cumulatedElapsedTime += elapsedTime;

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
            // Count updates to avoid catching up too much
            // Normal CPU lag will make program catch up 1 or 2 physics frames
            // but freezing or breaking into code for debug will produce a huge
            // cumulatedElapsedTime, and we don't want to catch up many frames.
            u8 updatesCount = 0;

            while (cumulatedElapsedTime >= sf::Time::Zero &&
                updatesCount < mp_maxUpdatesPerRender)
            {
                cumulatedElapsedTime -= mp_frameDuration;
                m_applicationTime += mp_frameDuration;

                // anti-overflow (brutal)
                if (m_applicationTime.asSeconds() > 1000 * 1000)
                {
                    m_applicationTime = sf::Time::Zero;
                }

                update(mp_frameDuration);
                ++updatesCount;
            }

            if (cumulatedElapsedTime >= sf::Time::Zero)
            {
                // There were still frames to catch up but we hit the max updates limit.
                // Still clear the cumulated elapsed time so we start afresh after a freeze,
                // instead of continuously trying to catchup as much as possible every frame,
                // which would cause the game to accelerate.
                cumulatedElapsedTime = sf::Time::Zero;
            }

            render();
        }
    }

    mc_window->close();
}

void GameApplication::update(sf::Time deltaTime)
{
    // update managers
    mc_inputManager->update();
    mc_dialogueManager->handleInput();

    // update characters
    mc_world->update(deltaTime);

    // move camera
    mc_view->move(0.f, std::sin(m_applicationTime.asSeconds()) * 50.f * deltaTime.asSeconds());
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
    mc_uiCanvas->render(*mc_window);

    // flip
    mc_window->display();
}
