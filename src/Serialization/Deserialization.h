#pragma once

// std
#include <memory>

// Game
#include "Common.h"

// yaml-cpp
namespace YAML
{
    class Node;
}

// Game
class GameApplication;
class SpatialObject;

namespace Deserialization
{
    std::unique_ptr<SpatialObject> deserialize(GameApplication& gameApp, const YAML::Node& spatialObjectNode);
}
