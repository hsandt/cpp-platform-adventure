#include "Application/GameApplication.h"

// SFML
#include <SFML/Graphics.hpp>

// RmlUi
#include <RmlUi/Core.h>
#if DEBUG
#include <RmlUi/Debugger.h>
#endif

// Game
#include "Application/AppConfig.h"
#include "Application/GameStateManager.h"
#include "Application/WindowConfig.h"
#include "Audio/MusicManager.h"
#include "Dialogue/DialogueManager.h"
#include "GameStates/InGameState.h"
#include "GameStates/TitleMenuState.h"
#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"
#include "Space/World.h"
#include "UI/RmlUIInterfaces/RenderInterfaceSFML.h"
#include "UI/RmlUIInterfaces/SystemInterfaceSFML.h"

GameApplication::GameApplication() :
    mr_rmlContext(),
    mc_window(),
    mc_renderTexture(),
    mc_view(),
    mc_rmlUiSystemInterface(),
    mc_rmlUiRenderer(),
    mc_world(*this),
    mc_gameStateManager(*this),
    mc_inputManager(*this),
    mc_textureManager(*this),
    mc_musicManager(*this),
    mc_dialogueManager(*this),
    mp_frameDuration(),
    mp_maxUpdatesPerRender(0),
    ms_applicationTime()
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
    initRmlUi();

    // Dialogue manager must be initialized after RmlUi
    mc_dialogueManager->init();

    initGameStateManager(appConfig.initialSceneName);
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

void GameApplication::initRmlUi()
{
    // inject window
    mc_rmlUiRenderer->SetWindow(mc_window.get());

    // inject interfaces
    Rml::SetSystemInterface(mc_rmlUiSystemInterface.get());
    Rml::SetRenderInterface(mc_rmlUiRenderer.get());

    // initialize
    #if PPK_ASSERT_ENABLED
    bool success =
        #endif
        Rml::Initialise();
    PPK_ASSERT_FATAL(success, "Could not initialize RmlUi");

    struct FontFace
    {
        Rml::String filename;
        bool fallback_face;
    };
    FontFace font_faces[] = {
        { "open-sans/OpenSans-Regular.ttf",         false },
        { "RmlUI samples/LatoLatin-Regular.ttf",    false },
        { "RmlUI samples/LatoLatin-Italic.ttf",     false },
        { "RmlUI samples/LatoLatin-Bold.ttf",       false },
        { "RmlUI samples/LatoLatin-BoldItalic.ttf", false },
        { "RmlUI samples/NotoEmoji-Regular.ttf",    true  },
    };

    for (const FontFace& face : font_faces)
    {
        Rml::LoadFontFace("assets/fonts/" + face.filename, face.fallback_face);
    }

    // create context to match window
    mr_rmlContext = Rml::CreateContext("default",
        Rml::Vector2i(mc_window->getSize().x, mc_window->getSize().y));
    PPK_ASSERT_FATAL(mr_rmlContext, "Could not create RmlUi context");

    #if DEBUG
    // initialize debugger
    Rml::Debugger::Initialise(mr_rmlContext.get());
    #endif
}

void GameApplication::initGameStateManager(const std::string& initialSceneName)
{
    // add Title Menu state
    auto titleMenuState = std::make_unique<TitleMenuState>(*this);
    mc_gameStateManager->addGameState(std::move(titleMenuState));

    // add In-Game state with initial scene to load
    auto inGameState = std::make_unique<InGameState>(*this);
    inGameState->mp_initialSceneName = initialSceneName;
    mc_gameStateManager->addGameState(std::move(inGameState));

    // start by entering InGame state
    mc_gameStateManager->queryEnterGameState((u8) GameStateID::TitleMenu);
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
            switch (event.type)
            {
                case sf::Event::MouseMoved:
                {
                    mr_rmlContext->ProcessMouseMove(event.mouseMove.x, event.mouseMove.y,
                        mc_rmlUiSystemInterface->GetKeyModifiers());
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    mr_rmlContext->ProcessMouseButtonDown(event.mouseButton.button,
                        mc_rmlUiSystemInterface->GetKeyModifiers());
                    break;
                }
                case sf::Event::MouseButtonReleased:
                {
                    mr_rmlContext->ProcessMouseButtonUp(event.mouseButton.button,
                        mc_rmlUiSystemInterface->GetKeyModifiers());
                    break;
                }
                case sf::Event::MouseWheelMoved:
                {
                    mr_rmlContext->ProcessMouseWheel(float(-event.mouseWheel.delta),
                        mc_rmlUiSystemInterface->GetKeyModifiers());
                    break;
                }
                case sf::Event::TextEntered:
                {
                    if (event.text.unicode > 32)
                    {
                        mr_rmlContext->ProcessTextInput(Rml::Character(event.text.unicode));
                    }
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    switch (event.key.code)
                    {
                        #if DEBUG
                        case sf::Keyboard::F8:
                        {
                            Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
                            break;
                        }
                        #endif
                        case sf::Keyboard::Escape:
                        {
                            shouldRun = false;
                            break;
                        }
                        default:
                            break;
                    }

                    mr_rmlContext->ProcessKeyDown(mc_rmlUiSystemInterface->TranslateKey(event.key.code),
                        mc_rmlUiSystemInterface->GetKeyModifiers());
                    break;
                }
                case sf::Event::KeyReleased:
                {

                    mr_rmlContext->ProcessKeyUp(mc_rmlUiSystemInterface->TranslateKey(event.key.code),
                        mc_rmlUiSystemInterface->GetKeyModifiers());
                    break;
                }
                case sf::Event::Closed:
                {
                    shouldRun = false;
                    break;
                }
                default:
                    break;
            }

            if (shouldRun)
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

    shutdown();
}

void GameApplication::update(sf::Time deltaTime)
{
    // update managers
    mc_inputManager->update();
    mc_gameStateManager->update();
    mc_dialogueManager->handleInput();

    // update characters
    mc_world->update(deltaTime);

    // update UI
    mr_rmlContext->Update();
}

void GameApplication::render()
{
    // set view from moving camera
    mc_renderTexture->setView(*mc_view);

    // render world
    mc_world->render(*mc_renderTexture);

    // set view back to default view (top-left origin) so UI has fixed position on screen
    mc_renderTexture->setView(mc_window->getDefaultView());

    // display render texture is important to prevent Y mirroring
    mc_renderTexture->display();

    // clear window
    mc_window->clear();

    // draw render texture on window (includes upscaling)
    sf::Sprite sprite(mc_renderTexture->getTexture());
    mc_window->draw(sprite);

    // render RmlUi
    // Normally, we should add its content to mc_renderTexture before it is
    // added to the window, to get pixelated upscaled output like the rest.
    // But RmlUiSFMLRenderer (from the samples) currently takes a RenderWindow only
    // so we must draw directly on the window, *after* the texture sprite.
    // We could adapt its code to take a RenderTarget so we can render RmlUi
    // directly on the native resolution texture, but remember we'll need to scale
    // all the UI down by windowConfig.upscaleFactor if doing so.
    mr_rmlContext->Render();

    // flip
    mc_window->display();
}

void GameApplication::shutdown()
{
    // clear references to objects about to be destroyed
    mr_rmlContext = nullptr;

    Rml::Shutdown();

    mc_window->close();
}
