#pragma once

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Common.h"
#include "Serialization/YamlHelper.hpp"

// Specialization of YAML convert for custom types

namespace YAML
{
    /// Vector<T> node converter
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

    /// Rect<T> node converter
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

    /// Color node converter
    template<>
    struct convert<sf::Color>
    {
        static Node encode(const sf::Color& rhs)
        {
            Node node;
            node["r"] = rhs.r;
            node["g"] = rhs.g;
            node["b"] = rhs.b;
            node["a"] = rhs.a;  // always store a when encoding
            return node;
        }

        static bool decode(const Node& node, sf::Color& rhs)
        {
            if (!node.IsMap() || !(node.size() >= 3 && node.size() <= 4))
            {
                return false;
            }

            rhs.r = node["r"].as<u8>();
            rhs.g = node["g"].as<u8>();
            rhs.b = node["b"].as<u8>();

            // alpha is optional on decode, so if you find it, set it,
            // but if you don't, default to opaque (255)
            if (!YamlHelper::tryGet<u8>(node, "a", rhs.a))
            {
                rhs.a = 255;
            }

            return true;
        }
    };
}
