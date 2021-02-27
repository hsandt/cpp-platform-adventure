#pragma once

// std
#include <concepts>

// SFML
#include <SFML/System.hpp>

// yaml-cpp
#include "yaml-cpp/yaml.h"

namespace YamlHelper
{
    /// Return the value at key in node
    /// UB unless key is found in node.
    /// This is useful to avoid a manual as<T>() conversion when obtaining a leaf value,
    /// but when just accessing another generic node from a node, getting node[key] is easier.
    /// YAMLKey type can be deduced from the node argument, so just pass <YAMLValue> on call
    /// (int, std::string, etc.)
    template<typename YAMLValue, typename YAMLKey>
    inline YAMLValue get(const YAMLKey& key, const YAML::Node& node)
    {
        // count on as<> to throw if key is invalid
        // return node[key].as<YAMLValue>();

        YAML::Node childNode = node[key];
        if (childNode.IsDefined())
        {
            // implicit conversion from Value to Out type may happen here
            return childNode.as<YAMLValue>();
        }

        // This requires key to be formattable to strings
        // but we only use string keys, so this should be fine.
        throw std::runtime_error(fmt::format("YamlHelper::get: no key '{}' found on node", key));

    }

    /// Try to get the value at key in node and store it in var
    /// If key is valid, set var by reference to YAML value at key and return true.
    /// Else, return false.
    /// YAMLValue must be convertible to Var type. Unlike get, we allow conversion
    /// because we cannot just cast the return value when using an out parameter.
    /// Var and YAMLKey types can be deduced from the arguments, so just pass <YAMLValue> on call
    /// (int, std::string, etc.)
    template<typename YAMLValue, typename Var, typename YAMLKey>
    requires std::convertible_to<YAMLValue, Var>
    inline bool tryGet(Var& var, const YAMLKey& key, const YAML::Node& node)
    {
        YAML::Node childNode = node[key];
        if (childNode.IsDefined())
        {
            // implicit conversion from Value to Out type may happen here
            var = childNode.as<YAMLValue>();
            return true;
        }

        return false;
    }

    /// Return YAML node as a Vector2f
    /// UB unless node contains 2 sequential elements
    inline sf::Vector2f asVector2f(const YAML::Node& node)
    {
        float x = YamlHelper::get<float>("x", node);
        float y = YamlHelper::get<float>("y", node);
        return sf::Vector2f(x, y);
    }
}
