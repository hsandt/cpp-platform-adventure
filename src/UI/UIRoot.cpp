#include "UI/UIRoot.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// Game
#include "UI/UIWidget.h"

UIRoot::UIRoot() :
    m_nextInsertHandle(0)
{
}

// Even empty, destructor definition in .cpp required if only class-forwarding
// some smart pointer contained types in .h
UIRoot::~UIRoot()
{
}

void UIRoot::render(sf::RenderWindow& window)
{
    for (const auto& [handle, widget] : m_widgets)
    {
        widget->render(window);
    }
}

Handle UIRoot::addWidget(std::unique_ptr<UIWidget> widget)
{
    m_widgets.insert({m_nextInsertHandle, std::move(widget)});
    return m_nextInsertHandle++;
}

std::optional<std::reference_wrapper<UIWidget>> UIRoot::getWidget(Handle widgetHandle) const
{
    auto it = m_widgets.find(widgetHandle);
    return it != m_widgets.end() ? std::optional{std::ref(*it->second)} : std::nullopt;
}

void UIRoot::removeWidget(std::optional<Handle>& oWidgetHandle)
{
    size_t erasedCount = m_widgets.erase(*oWidgetHandle);

    if (erasedCount == 0)
    {
        throw std::runtime_error(fmt::format(
            "No widget with handle {} found in UIRoot::m_widgets, could not remove.",
            *oWidgetHandle
        ));
    }

    // clear handle for safety
    oWidgetHandle.reset();
}
