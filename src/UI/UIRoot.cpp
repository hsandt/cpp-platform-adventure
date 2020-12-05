#include "UI/UIRoot.h"

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

bool UIRoot::removeWidget(Handle widgetHandle)
{
    size_t erasedCount = m_widgets.erase(widgetHandle);
    return erasedCount > 0;
}