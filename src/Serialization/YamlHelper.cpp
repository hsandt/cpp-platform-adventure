#include "Serialization/YamlHelper.h"

namespace YamlHelper
{
    /// Return YAML node as a Color with max opacity
    /// UB unless node contains r, g and b as float
    sf::Color asColor(const YAML::Node& node)
    {
        float r = node["r"].as<float>();
        float g = node["g"].as<float>();
        float b = node["b"].as<float>();
        return sf::Color(r, g, b);
    }
}
