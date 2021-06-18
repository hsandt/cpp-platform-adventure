#pragma once

// std
#include <filesystem>
#include <map>
#include <string>

// Game
#include "Common.h"
#include "Application/ApplicationObject.h"

// SFML
namespace sf
{
    class Texture;
}

class TextureManager : public ApplicationObject
{
public:

    explicit TextureManager(GameApplication& gameApp);
    ~TextureManager();

    /// Load texture from relative file path
    const sf::Texture& loadFromFile(const std::string& relativeFilePathString);

public:

    /// Path to texture assets directory
    static std::filesystem::path textureAssetsDirPath;

    /* State */

    /// Map of loaded textures, indexed by relative file path
    std::map<std::string, sf::Texture> ms_textureMap;
};
