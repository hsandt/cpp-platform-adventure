#pragma once

#include <memory>

namespace sf
{
    class RenderTarget;
}

/// Abstract base class for all UI widgets
class UIWidget
{
public:
    UIWidget();
    virtual ~UIWidget() = 0;

    /// Render this widget
    virtual void render(sf::RenderTarget& renderTarget) = 0;

private:

    /* Components */
};
