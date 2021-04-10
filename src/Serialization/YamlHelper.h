#pragma once

// std
#include <concepts>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// yaml-cpp
#include "yaml-cpp/yaml.h"

// Game
#include "Common.h"

namespace YamlHelper
{
    /// Try to get the value at key in node and store it in var
    /// If key is valid, set var by reference to YAML value at key and return true.
    /// Else, return false.
    /// YAMLValue must be convertible to Var type. Unlike get, we allow conversion
    /// because we cannot just cast the return value when using an out parameter.
    /// Var and YAMLKey types can be deduced from the arguments, so just pass <YAMLValue> on call
    /// (int, std::string, etc.)
    template<typename YAMLValue, typename Var, typename YAMLKey>
    requires std::convertible_to<YAMLValue, Var>
    inline bool tryGet(const YAML::Node& node, const YAMLKey& key, Var& outVar)
    {
        YAML::Node childNode = node[key];
        if (childNode.IsDefined())
        {
            // implicit conversion from Value to Out type may happen here
            outVar = childNode.as<YAMLValue>();
            return true;
        }

        return false;
    }

    /// Return YAML node as a Vector2f
    /// UB unless node contains x and y values as float
    sf::Vector2f asVector2f(const YAML::Node& node);

    /// Return YAML node as a Color with max opacity
    /// UB unless node contains r, g and b as float
    sf::Color asColor(const YAML::Node& node);
}
