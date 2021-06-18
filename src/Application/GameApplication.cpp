#include "Application/GameApplication.h"

// PPK_ASSERT
#include "ppk_assert.h"

// SFML
#include <SFML/Graphics.hpp>

// Game
#include "Application/AppConfig.h"
#include "Application/GameStateManager.h"
#include "Application/WindowConfig.h"
#include "Audio/MusicManager.h"
#include "Dialogue/DialogueManager.h"
#include "GameStates/InGameState.h"
#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"
#include "UI/UICanvas.h"
#include "Space/World.h"

GameApplication::GameApplication() :
    mc_window(),
    mc_renderTexture(),
    mc_view(),
    mc_world(*this),
    mc_uiCanvas(*this),
    mc_gameStateManager(*this),
    mc_inputManager(*this),
    mc_textureManager(*this),
    mc_musicManager(*this),
    mc_dialogueManager(*this)
{
}

GameApplication::~GameApplication()
{
}

void GameApplication::initAndRun()
{
    init();
    run();
}

void GameApplication::init()
{
    // retrieve App Config from config file and derive parameters
    AppConfig appConfig = AppConfig::fromFile("config/app_config.yml");
    mp_frameDuration = sf::seconds(1.f / appConfig.fps);
    mp_maxUpdatesPerRender = appConfig.maxUpdatesPerRender;

    initWindow();
    initGameStateManager();

    // load initial scene
    mc_world->deferLoadScene(appConfig.initialSceneName);

    // set initial input context to Platforming
    mc_inputManager->pushInputContext(InputContext::Platforming);
}

void GameApplication::initWindow()
{
    // retrieve Window Config from config file
    WindowConfig windowConfig = WindowConfig::fromFile("config/window_config.yml");

    // set aliasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = windowConfig.antialiasingLevel;

    // create window (closable, not resizable) with native resolution
    mc_window->create(sf::VideoMode(windowConfig.nativeWidth, windowConfig.nativeHeight), windowConfig.title, sf::Style::Close, settings);

    // resize window to match upscaling factor (the unsmoothed render texture will make sure we get nearest-neighbor scaling)
    u16 windowWidth = windowConfig.upscaleFactor * windowConfig.nativeWidth;
    u16 windowHeight = windowConfig.upscaleFactor * windowConfig.nativeHeight;
    mc_window->setSize(sf::Vector2u(windowWidth, windowHeight));

    // center window on screen by getting current desktop resolution and placing window top-left accordingly
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    mc_window->setPosition(sf::Vector2i((desktopMode.width - windowWidth) / 2, (desktopMode.height - windowHeight) / 2));

    // disable key repeat (this is not tunable in WindowConfig because most games either don't use it
    // or implement their own repeat detection system)
    mc_window->setKeyRepeatEnabled(false);

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

    #if PPK_ASSERT_ENABLED
    bool success =
    #endif
    mc_renderTexture->create(windowConfig.nativeWidth, windowConfig.nativeHeight);
    PPK_ASSERT_FATAL(success, "Could not create render texture");

    // create camera view (centered)
    mc_view->setCenter(sf::Vector2f(0.f, 0.f));
    mc_view->setSize(sf::Vector2f(windowConfig.nativeWidth, windowConfig.nativeHeight));
}

void GameApplication::initGameStateManager()
{
    mc_gameStateManager->addGameState(std::make_unique<InGameState>(*this));
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

                // Don't worry about overflow, we store microseconds as i64,
                // which can last for 146,000 years
                ms_applicationTime += mp_frameDuration;

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
}

void GameApplication::render()
{
    // clear sky
    mc_renderTexture->clear(sf::Color::Cyan);

    // set view from moving camera
    mc_renderTexture->setView(*mc_view);

    // render world
    mc_world->render(*mc_renderTexture);

    // set view back to default view (top-left origin) so UI has fixed position on screen
    mc_renderTexture->setView(mc_window->getDefaultView());

    // render UI
    mc_uiCanvas->render(*mc_renderTexture);

    // display render texture is important to prevent Y mirroring
    mc_renderTexture->display();

    // clear window
    mc_window->clear();

    // draw render texture on window (includes upscaling)
    sf::Sprite sprite(mc_renderTexture->getTexture());
    mc_window->draw(sprite);

    // flip
    mc_window->display();
}
