#include "Audio/MusicManager.h"

// PPK_ASSERT
#include "ppk_assert.h"

// fmt
#include "fmt/format.h"

// SFML
#include <SFML/Audio.hpp>

// Game

/* static */ std::filesystem::path MusicManager::bgmAssetsDirPath("assets/audio/bgm");

MusicManager::MusicManager(GameApplication& gameApp) :
    ApplicationObject(gameApp),
    ms_music(),
    ms_oCurrentBgmRelativePathString()
{
}

MusicManager::~MusicManager()
{
}

void MusicManager::playBgm(const std::string& relativeFilePathString)
{
    // do not replay the same BGM from start, as we often have scenes with the same BGM
    // connected to each other
    if (ms_oCurrentBgmRelativePathString != relativeFilePathString)
    {
        std::filesystem::path filePath = bgmAssetsDirPath / relativeFilePathString;

        bool result = ms_music.openFromFile(filePath);
        PPK_ASSERT_DEBUG(result, "Could not open bgm at file path: %s", filePath.c_str());

        if (result)
        {
            ms_music.setLoop(true);
            ms_music.play();

            ms_oCurrentBgmRelativePathString = relativeFilePathString;
        }
    }
}
