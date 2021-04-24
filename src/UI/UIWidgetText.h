#pragma once

#include <memory>

#include "UI/UIWidget.h"

namespace sf
{
    class RenderTarget;
    class Text;
}

struct Transform;

/// UI widget representing a text
class UIWidgetText : public UIWidget
{
public:
    UIWidgetText();
    ~UIWidgetText();

    /// Render this widget as a text
    void render(sf::RenderTarget& renderTarget);

public:

    /* Components */

    const std::unique_ptr<Transform> mc_transform;
    const std::unique_ptr<sf::Text> mc_text;

    /* Parameters */

    std::string mp_text;
};
