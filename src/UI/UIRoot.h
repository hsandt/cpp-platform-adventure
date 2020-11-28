#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

#include "Common.h"

class UIWidget;

class UIRoot
{
public:
    UIRoot();
    ~UIRoot();

public:
    /// Add widget to map of widgets, and return map handle
    /// for future access. This transfers ownership to UI root
    Handle addWidget(std::unique_ptr<UIWidget> widget);

    /// Return widget with handle, or empty unique_ptr if none matches
    std::optional<std::reference_wrapper<UIWidget>> getWidget(Handle widgetHandle) const;

    /// Remove widget with handle, if one is found
    /// Return whether the value was in the map
    bool removeWidget(Handle widgetHandle);

private:

    /* State */

    /// Handle to insert the next widget at
    Handle m_nextInsertHandle;

    /// Map of owned widgets, identified by handle
    std::unordered_map<Handle, std::unique_ptr<UIWidget>> m_widgets;
};
