#pragma once

// std
#include <filesystem>
#include <optional>
#include <string>

// SFML
#include <SFML/Audio.hpp>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"

class MusicManager : public ApplicationObject
{
public:

    explicit MusicManager(GameApplication& gameApp);
    ~MusicManager();

public:

    /// Path to BGM assets directory
    static std::filesystem::path bgmAssetsDirPath;

    /// Play looping BGM from relative file path, unless it is already played
    void playBgm(const std::string& relativeFilePathString);

    /* State */

    /// Current music stream
    sf::Music ms_music;

    /// Relative file path string of current BGM played
    std::optional<std::string> ms_oCurrentBgmRelativePathString;
};
