#include "Serialization/Deserialization.h"

// std
#include <string>

// fmt
#include "fmt/format.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Entities/NonPlayerCharacter.h"
#include "Entities/PickUpItem.h"
#include "Entities/PlayerCharacter.h"

namespace Deserialization
{
    std::unique_ptr<SpatialObject> deserialize(GameApplication& gameApp, const YAML::Node& spatialObjectNode)
    {
        auto type = spatialObjectNode["type"].as<std::string>();
        auto id = spatialObjectNode["id"].as<Handle>();
        std::unique_ptr<SpatialObject> spatialObject;

        if (type == "PlayerCharacter")
        {
            spatialObject = std::make_unique<PlayerCharacter>(gameApp, id);
        }
        else if (type == "NonPlayerCharacter")
        {
            spatialObject = std::make_unique<NonPlayerCharacter>(gameApp, id);
        }
        else if (type == "PickUpItem")
        {
            spatialObject = std::make_unique<PickUpItem>(gameApp, id);
        }
        else
        {
            PPK_ASSERT_DEBUG(false, "Unsupported type %s, deserialize will return empty unique_ptr.", type.c_str());
            return nullptr;
        }

        spatialObject->deserialize(spatialObjectNode);
        return spatialObject;
    }
}
