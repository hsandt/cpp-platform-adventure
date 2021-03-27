#pragma once

// std
#include <filesystem>

// SFML
#include <SFML/Audio.hpp>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"

class MusicManager : protected ApplicationObject
{
public:

    explicit MusicManager(GameApplication& gameApp);
    ~MusicManager();

public:

    /// Path to BGM assets directory
    static std::filesystem::path bgmAssetsDirPath;

    /// Play looping BGM from relative file path
    void playBgm(const std::string& relativeFilePathString);

    /* State */

    /// Current music stream
    sf::Music ms_music;
};
