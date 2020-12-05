#pragma once

#include <memory>

#include "UI/UIWidget.h"

namespace sf
{
    class RenderWindow;
    class Text;
}

class Transform;

/// UI widget representing a text
class UIWidgetText : public UIWidget
{
public:
    UIWidgetText();
    ~UIWidgetText();

    /// Render this widget as a text
    void render(sf::RenderWindow& window);

public:

    /* Components */

    const std::unique_ptr<Transform> mc_transform;
    const std::unique_ptr<sf::Text> mc_text;

    /* Parameters */

    std::string mp_text;
};
