#include "imgui_style.h"

void ImGuiStyle::ScaleAllSizes(float scale_factor)
{
    WindowPadding = ImFloor(WindowPadding * scale_factor);
    WindowRounding = std::floor(WindowRounding * scale_factor);
    WindowMinSize = ImFloor(WindowMinSize * scale_factor);
    ChildRounding = std::floor(ChildRounding * scale_factor);
    PopupRounding = std::floor(PopupRounding * scale_factor);
    FramePadding = ImFloor(FramePadding * scale_factor);
    FrameRounding = std::floor(FrameRounding * scale_factor);
    ItemSpacing = ImFloor(ItemSpacing * scale_factor);
    ItemInnerSpacing = ImFloor(ItemInnerSpacing * scale_factor);
    CellPadding = ImFloor(CellPadding * scale_factor);
    TouchExtraPadding = ImFloor(TouchExtraPadding * scale_factor);
    IndentSpacing = std::floor(IndentSpacing * scale_factor);
    ScrollbarSize = std::floor(ScrollbarSize * scale_factor);
    ScrollbarRounding = std::floor(ScrollbarRounding * scale_factor);
    GrabMinSize = std::floor(GrabMinSize * scale_factor);
    GrabRounding = std::floor(GrabRounding * scale_factor);
    LogSliderDeadzone = std::floor(LogSliderDeadzone * scale_factor);
    TabRounding = std::floor(TabRounding * scale_factor);
    TabMinWidthForCloseButton = (TabMinWidthForCloseButton != FLT_MAX) ? std::floor(TabMinWidthForCloseButton * scale_factor) : FLT_MAX;
    SeparatorTextPadding = ImFloor(SeparatorTextPadding * scale_factor);
    DisplayWindowPadding = ImFloor(DisplayWindowPadding * scale_factor);
    DisplaySafeAreaPadding = ImFloor(DisplaySafeAreaPadding * scale_factor);
    MouseCursorScale = std::floor(MouseCursorScale * scale_factor);
}


void ImGuiStyle::PushColor(ImGuiCol idx, ImColorf col)
{
    ColorStack.push_back({ idx, Colors[idx] });
    Colors[idx] = col;
}

void ImGuiStyle::PushColor(ImGuiCol idx, ImCol col)
{
    PushColor(idx, ImGui::ColorConvertToFloat(col));
}

void ImGuiStyle::PopColor(size_t count)
{
    while (count > 0) {
        const auto& [idx, col] = ColorStack.back();
        Colors[idx] = col;
        ColorStack.pop_back();
        --count;
    }
}
