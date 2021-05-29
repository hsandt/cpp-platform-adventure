#include "Serialization/Deserialization.h"

// std
#include <stdexcept>
#include <string>

// fmt
#include "fmt/format.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Serialization/YamlHelper.h"
#include "Entities/NonPlayerCharacter.h"
#include "Entities/PickUpItem.h"
#include "Entities/PlayerCharacter.h"

namespace Deserialization
{
    std::unique_ptr<SpatialObject> deserialize(GameApplication& gameApp, const YAML::Node& spatialObjectNode)
    {
        auto type = spatialObjectNode["type"].as<std::string>();

        if (type == "PlayerCharacter")
        {
            return PlayerCharacter::deserialize(gameApp, spatialObjectNode);
        }
        else if (type == "NonPlayerCharacter")
        {
            return NonPlayerCharacter::deserialize(gameApp, spatialObjectNode);
        }
        else if (type == "PickUpItem")
        {
            return PickUpItem::deserialize(gameApp, spatialObjectNode);
        }
        else
        {
            PPK_ASSERT_DEBUG(false, "Unsupported type %s, deserialize will return empty unique_ptr.", type.c_str());
            return nullptr;
        }
    }
}
