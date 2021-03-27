#include "Audio/MusicManager.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// SFML
#include <SFML/Audio.hpp>

// Game

/* static */ std::filesystem::path MusicManager::bgmAssetsDirPath("assets/audio/bgm");

MusicManager::MusicManager(GameApplication& gameApp) :
    ApplicationObject(gameApp)
{
}

MusicManager::~MusicManager()
{
}

void MusicManager::playBgm(const std::string& relativeFilePathString)
{
    std::filesystem::path filePath = bgmAssetsDirPath / relativeFilePathString;

    bool result = ms_music.openFromFile(filePath);
    if (!result)
    {
        throw std::runtime_error(fmt::format("Could not open bgm at file path: %s", filePath.c_str()));
    }

    ms_music.setLoop(true);
    ms_music.play();
}
