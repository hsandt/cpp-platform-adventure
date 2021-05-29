#include "Serialization/YamlHelper.h"

namespace YamlHelper
{
    /// Return YAML node as a Vector2f
    /// UB unless node contains x and y values as float
    sf::Vector2f asVector2f(const YAML::Node& node)
    {
        float x = node["x"].as<float>();
        float y = node["y"].as<float>();
        return sf::Vector2f(x, y);
    }

    /// Return YAML node as a Vector2f
    /// UB unless node contains x and y values as float
    sf::IntRect asIntRect(const YAML::Node& node)
    {
        int x = node["x"].as<int>();
        int y = node["y"].as<int>();
        int w = node["w"].as<int>();
        int h = node["h"].as<int>();
        return sf::IntRect(x, y, w, h);
    }

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
