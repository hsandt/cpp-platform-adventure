#include "Serialization/Deserialization.h"

// std
#include <stdexcept>
#include <string>

// fmt
#include "fmt/format.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Serialization/YamlHelper.hpp"
#include "Entities/NonPlayerCharacter.h"
#include "Entities/PickUpItem.h"
#include "Entities/PlayerCharacter.h"

namespace Deserialization
{
    std::unique_ptr<SpatialObject> deserialize(GameApplication& gameApp, const YAML::Node& spatialObjectNode)
    {
        auto type = spatialObjectNode["type"].as<std::string>();
        auto id = spatialObjectNode["id"].as<Handle>();

        if (type == "PlayerCharacter")
        {
            auto playerCharacter = std::make_unique<PlayerCharacter>(gameApp, id);
            playerCharacter->deserialize(spatialObjectNode);
            return playerCharacter;
        }
        else if (type == "NonPlayerCharacter")
        {
            auto nonPlayerCharacter = std::make_unique<NonPlayerCharacter>(gameApp, id);
            nonPlayerCharacter->deserialize(spatialObjectNode);
            return nonPlayerCharacter;
        }
        else if (type == "PickUpItem")
        {
            auto pickUpItem = std::make_unique<PickUpItem>(gameApp, id);
            pickUpItem->deserialize(spatialObjectNode);
            return pickUpItem;
        }
        else
        {
            PPK_ASSERT_DEBUG(false, "Unsupported type %s, deserialize will return empty unique_ptr.", type.c_str());
            return nullptr;
        }
    }
}
