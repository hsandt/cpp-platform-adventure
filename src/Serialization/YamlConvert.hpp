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
            node["x"] = rhs.x;
            node["y"] = rhs.y;
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

    template<typename T>
    struct convert<sf::Rect<T>>
    {
        static Node encode(const sf::Rect<T>& rhs)
        {
            Node node;
            node["left"] = rhs.left;
            node["top"] = rhs.top;
            node["width"] = rhs.width;
            node["height"] = rhs.height;
            return node;
        }

        static bool decode(const Node& node, sf::Rect<T>& rhs)
        {
            if (!node.IsMap() || node.size() != 4)
            {
                return false;
            }

            rhs.left = node["left"].as<T>();
            rhs.top = node["top"].as<T>();
            rhs.width = node["width"].as<T>();
            rhs.height = node["height"].as<T>();
            return true;
        }
    };
}
