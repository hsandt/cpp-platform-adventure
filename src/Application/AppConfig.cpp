#include "AppConfig.h"

#include <stdexcept>

#include "fmt/format.h"
#include "yaml-cpp/yaml.h"

/* static */ AppConfig AppConfig::from_file(const std::string& filename)
{
    AppConfig appConfig;

    try
    {
        YAML::Node appConfigFile = YAML::LoadFile(filename);

        // Support for u8 is added in commit "Support as<uint8_t>/as<int8_t>."
        // https://github.com/jbeder/yaml-cpp/commit/4dbfeb0bbccac8164ab8f8686307867c0729d8c1
        // but v0.6.3 is behind so for now, just parse as int.
        try_set_from_key<int>(appConfig.fps, "fps", appConfigFile);
    }
    catch(const YAML::BadFile& e)
    {
        // what() just contains "bad file", so prefer custom error message
        throw std::runtime_error(fmt::format("YAML::BadFile: '{}'", filename));
    }

    return appConfig;
}

template<typename YAMLValue, typename Var, typename YAMLKey>
requires std::convertible_to<YAMLValue, Var>
/* static */ bool AppConfig::try_set_from_key(Var& var, const YAMLKey& key, const YAML::Node& appConfigFile)
{
    YAML::Node node = appConfigFile[key];
    if (node.IsDefined())
    {
        // implicit conversion from Value to Out type may happen here
        var = node.as<YAMLValue>();
        return true;
    }

    return false;
}

AppConfig::AppConfig() :
    fps(60)
{
}
