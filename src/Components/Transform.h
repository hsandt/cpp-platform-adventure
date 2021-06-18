#pragma once

// SFML
#include <SFML/System.hpp>

// yaml-cpp
namespace YAML
{
    class Node;
}

/// Transform component
/// Handles position
struct Transform
{
    sf::Vector2f position;

    void deserialize(const YAML::Node& transformNode);
};
