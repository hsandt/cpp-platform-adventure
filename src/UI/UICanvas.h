#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <map>

#include "Common.h"

namespace sf
{
    class RenderWindow;
}

class UIWidget;

class UICanvas
{
public:
    UICanvas();
    ~UICanvas();

public:

    /// Render all widgets, elements with greatest Handle at the top
    void render(sf::RenderWindow& window);

    /// Add widget to map of widgets, and return map handle
    /// for future access. This transfers ownership to UI root
    Handle addWidget(std::unique_ptr<UIWidget> widget);

    /// Return widget with handle, or empty unique_ptr if none matches
    std::optional<std::reference_wrapper<UIWidget>> getWidget(Handle widgetHandle) const;

    /// Remove widget with handle and clear optional handle
    /// This invalidates the handle, and clearing the handle passed by reference is for safety,
    /// but if you have another copy of that handle, make sure to clear it manually afterward.
    /// UB unless a widget with the passed handle exists.
    void removeWidget(std::optional<Handle>& oWidgetHandle);

private:

    /* State */

    /// Handle to insert the next widget at
    Handle m_nextInsertHandle;

    /// Map of owned widgets, identified by handle
    std::map<Handle, std::unique_ptr<UIWidget>> m_widgets;
};
