#include "Components/Transform.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Serialization/YamlConvert.hpp"

void Transform::deserialize(const YAML::Node& transformNode)
{
    position = transformNode["position"].as<sf::Vector2f>();
}
