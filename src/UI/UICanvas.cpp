#include "UI/UICanvas.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// Game
#include "UI/UIWidget.h"

UICanvas::UICanvas() :
    m_nextInsertHandle(0)
{
}

// Even empty, destructor definition in .cpp required if only class-forwarding
// some smart pointer contained types in .h
UICanvas::~UICanvas()
{
}

void UICanvas::render(sf::RenderWindow& window)
{
    for (const auto& [handle, widget] : m_widgets)
    {
        widget->render(window);
    }
}

Handle UICanvas::addWidget(std::unique_ptr<UIWidget> widget)
{
    m_widgets.insert({m_nextInsertHandle, std::move(widget)});
    return m_nextInsertHandle++;
}

std::optional<std::reference_wrapper<UIWidget>> UICanvas::getWidget(Handle widgetHandle) const
{
    auto it = m_widgets.find(widgetHandle);
    return it != m_widgets.end() ? std::optional{std::ref(*it->second)} : std::nullopt;
}

void UICanvas::removeWidget(std::optional<Handle>& oWidgetHandle)
{
    size_t erasedCount = m_widgets.erase(*oWidgetHandle);

    if (erasedCount == 0)
    {
        throw std::runtime_error(fmt::format(
            "No widget with handle {} found in UICanvas::m_widgets, could not remove.",
            *oWidgetHandle
        ));
    }

    // clear handle for safety
    oWidgetHandle.reset();
}
