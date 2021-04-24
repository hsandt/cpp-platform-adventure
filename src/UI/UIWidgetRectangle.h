#pragma once

#include <memory>

#include "UI/UIWidget.h"

namespace sf
{
    class RectangleShape;
    class RenderTarget;
}

struct Transform;

/// UI widget representing a rectangle of uniform color
class UIWidgetRectangle : public UIWidget
{
public:
    UIWidgetRectangle();
    ~UIWidgetRectangle();

    /// Render this widget as a rectangle of uniform color
    void render(sf::RenderTarget& renderTarget);

public:

    /* Components */

    const std::unique_ptr<Transform> mc_transform;
    const std::unique_ptr<sf::RectangleShape> mc_shape;
};
