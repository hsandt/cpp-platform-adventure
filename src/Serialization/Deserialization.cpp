#include "Serialization/Deserialization.h"

// std
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
        std::string type = YamlHelper::get<std::string>(spatialObjectNode, "type");

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
            throw std::runtime_error(fmt::format("Unsupported type {}", type));
        }
    }
}
