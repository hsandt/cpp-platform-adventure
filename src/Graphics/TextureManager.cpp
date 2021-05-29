#include "TextureManager.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// SFML
#include <SFML/Graphics.hpp>

/* static */ std::filesystem::path TextureManager::textureAssetsDirPath("assets/textures");

TextureManager::TextureManager(GameApplication& gameApp) :
    ApplicationObject(gameApp)
{
}

TextureManager::~TextureManager()
{
}

const sf::Texture& TextureManager::loadFromFile(const std::string& relativeFilePathString)
{
    std::filesystem::path filePath = textureAssetsDirPath / relativeFilePathString;

    // currently no lazy loading, just overwrite any existing entry, else create a default one
    sf::Texture& texture = ms_textureMap[relativeFilePathString];
    bool result = texture.loadFromFile(filePath);
    PPK_ASSERT_DEBUG(result, "Could not load texture at file path: %s", filePath.c_str());

    return texture;
}
