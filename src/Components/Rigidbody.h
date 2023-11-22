#pragma once

// SFML
#include <SFML/System.hpp>

// yaml-cpp
namespace YAML
{
  class Node;
}

/// Rigidbody component
/// Handles position
struct Rigidbody
{
  sf::Vector2f position;

  void deserialize(const YAML::Node& rigidbodyNode);
};
