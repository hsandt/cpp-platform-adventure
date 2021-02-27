#pragma once

#include <memory>

namespace sf
{
    class RenderWindow;
}

/// Abstract base class for all UI widgets
class UIWidget
{
public:
    UIWidget();
    virtual ~UIWidget() = 0;

    /// Render this widget
    virtual void render(sf::RenderWindow& window) = 0;

private:

    /* Components */
};
