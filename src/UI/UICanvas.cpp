#include "UI/UICanvas.h"

// std
#include <stdexcept>

// fmt
#include "fmt/format.h"

// Game
#include "UI/UIWidget.h"

UICanvas::UICanvas(GameApplication& gameApp) :
    ApplicationObject(gameApp),
    ms_nextInsertHandle(0)
{
}

// Even empty, destructor definition in .cpp required if only class-forwarding
// some smart pointer contained types in .h
UICanvas::~UICanvas()
{
}

void UICanvas::render(sf::RenderTarget& renderTarget)
{
    for (const auto& [handle, widget] : ms_widgets)
    {
        widget->render(renderTarget);
    }
}

Handle UICanvas::addWidget(std::unique_ptr<UIWidget> widget)
{
    ms_widgets.insert({ms_nextInsertHandle, std::move(widget)});
    return ms_nextInsertHandle++;
}

std::optional<std::reference_wrapper<UIWidget>> UICanvas::getWidget(Handle widgetHandle) const
{
    auto it = ms_widgets.find(widgetHandle);
    return it != ms_widgets.end() ? std::optional{std::ref(*it->second)} : std::nullopt;
}

void UICanvas::removeWidget(std::optional<Handle>& oWidgetHandle)
{
    size_t erasedCount = ms_widgets.erase(*oWidgetHandle);

    if (erasedCount == 0)
    {
        throw std::runtime_error(fmt::format(
            "No widget with handle {} found in UICanvas::ms_widgets, could not remove.",
            *oWidgetHandle
        ));
    }

    // clear handle for safety
    oWidgetHandle.reset();
}
