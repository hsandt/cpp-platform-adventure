#include "AppConfig.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Serialization/YamlHelper.h"

/* static */ AppConfig AppConfig::fromFile(const std::string& filename)
{
    AppConfig appConfig;

    try
    {
        YAML::Node appConfigFile = YAML::LoadFile(filename);

        // Support for u8 is added in commit "Support as<uint8_t>/as<int8_t>."
        // https://github.com/jbeder/yaml-cpp/commit/4dbfeb0bbccac8164ab8f8686307867c0729d8c1
        // but v0.6.3 is behind so for now, just parse as int.
        YamlHelper::tryGet<int>(appConfigFile, "fps", appConfig.fps);
        YamlHelper::tryGet<int>(appConfigFile, "maxUpdatesPerRender", appConfig.maxUpdatesPerRender);
    }
    catch(const YAML::BadFile& e)
    {
        // what() just contains "bad file", so prefer custom error message
        throw std::runtime_error(fmt::format("YAML::BadFile: '{}'", filename));
    }

    return appConfig;
}

AppConfig::AppConfig() :
    fps(60),
    maxUpdatesPerRender(3)
{
}
