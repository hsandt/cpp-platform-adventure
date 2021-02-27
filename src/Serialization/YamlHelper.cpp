#include "Serialization/YamlHelper.h"

namespace YamlHelper
{
    /// Return YAML node as a Vector2f
    /// UB unless node contains x and y values as float
    sf::Vector2f asVector2f(const YAML::Node& node)
    {
        float x = YamlHelper::get<float>(node, "x");
        float y = YamlHelper::get<float>(node, "y");
        return sf::Vector2f(x, y);
    }
    /// Return YAML node as a Color with max opacity
    /// UB unless node contains r, g and b as float
    sf::Color asColor(const YAML::Node& node)
    {
        float r = YamlHelper::get<float>(node, "r");
        float g = YamlHelper::get<float>(node, "g");
        float b = YamlHelper::get<float>(node, "b");
        return sf::Color(r, g, b);
    }
}
