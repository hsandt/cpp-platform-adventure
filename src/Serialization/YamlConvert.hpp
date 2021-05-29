#pragma once

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Specialization of YAML convert for custom types

namespace YAML
{
    template<typename T>
    struct convert<sf::Vector2<T>>
    {
        static Node encode(const sf::Vector2<T>& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, sf::Vector2<T>& rhs)
        {
            if (!node.IsMap() || node.size() != 2)
            {
                return false;
            }

            rhs.x = node["x"].as<T>();
            rhs.y = node["y"].as<T>();
            return true;
        }
    };
}
