#pragma once

#include "imgui_types.h"

// Enumeration for PushStyleColor() / PopStyleColor()
enum ImGuiCol_
{
    ImGuiCol_Text,
    ImGuiCol_TextDisabled,
    ImGuiCol_WindowBg,              // Background of normal windows
    ImGuiCol_ChildBg,               // Background of child windows
    ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
    ImGuiCol_Border,
    ImGuiCol_BorderShadow,
    ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
    ImGuiCol_FrameBgHovered,
    ImGuiCol_FrameBgActive,
    ImGuiCol_TitleBg,
    ImGuiCol_TitleBgActive,
    ImGuiCol_TitleBgCollapsed,
    ImGuiCol_MenuBarBg,
    ImGuiCol_ScrollbarBg,
    ImGuiCol_ScrollbarGrab,
    ImGuiCol_ScrollbarGrabHovered,
    ImGuiCol_ScrollbarGrabActive,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    ImGuiCol_HeaderHovered,
    ImGuiCol_HeaderActive,
    ImGuiCol_Separator,
    ImGuiCol_SeparatorHovered,
    ImGuiCol_SeparatorActive,
    ImGuiCol_ResizeGrip,            // Resize grip in lower-right and lower-left corners of windows.
    ImGuiCol_ResizeGripHovered,
    ImGuiCol_ResizeGripActive,
    ImGuiCol_Tab,                   // TabItem in a TabBar
    ImGuiCol_TabHovered,
    ImGuiCol_TabActive,
    ImGuiCol_TabUnfocused,
    ImGuiCol_TabUnfocusedActive,
    ImGuiCol_PlotLines,
    ImGuiCol_PlotLinesHovered,
    ImGuiCol_PlotHistogram,
    ImGuiCol_PlotHistogramHovered,
    ImGuiCol_TableHeaderBg,         // Table header background
    ImGuiCol_TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
    ImGuiCol_TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
    ImGuiCol_TableRowBg,            // Table row background (even rows)
    ImGuiCol_TableRowBgAlt,         // Table row background (odd rows)
    ImGuiCol_TextSelectedBg,
    ImGuiCol_DragDropTarget,        // Rectangle highlighting a drop target
    ImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
    ImGuiCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB
    ImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
    ImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
    ImGuiCol_COUNT
};

enum ImGuiStyleVar_
{
    // Enum name --------------------- // Member in ImGuiStyle structure (see ImGuiStyle for descriptions)
    ImGuiStyleVar_Alpha,               // float     Alpha
    ImGuiStyleVar_DisabledAlpha,       // float     DisabledAlpha
    ImGuiStyleVar_WindowPadding,       // ImVec2    WindowPadding
    ImGuiStyleVar_WindowRounding,      // float     WindowRounding
    ImGuiStyleVar_WindowBorderSize,    // float     WindowBorderSize
    ImGuiStyleVar_WindowMinSize,       // ImVec2    WindowMinSize
    ImGuiStyleVar_WindowTitleAlign,    // ImVec2    WindowTitleAlign
    ImGuiStyleVar_ChildRounding,       // float     ChildRounding
    ImGuiStyleVar_ChildBorderSize,     // float     ChildBorderSize
    ImGuiStyleVar_PopupRounding,       // float     PopupRounding
    ImGuiStyleVar_PopupBorderSize,     // float     PopupBorderSize
    ImGuiStyleVar_FramePadding,        // ImVec2    FramePadding
    ImGuiStyleVar_FrameRounding,       // float     FrameRounding
    ImGuiStyleVar_FrameBorderSize,     // float     FrameBorderSize
    ImGuiStyleVar_ItemSpacing,         // ImVec2    ItemSpacing
    ImGuiStyleVar_ItemInnerSpacing,    // ImVec2    ItemInnerSpacing
    ImGuiStyleVar_IndentSpacing,       // float     IndentSpacing
    ImGuiStyleVar_CellPadding,         // ImVec2    CellPadding
    ImGuiStyleVar_ScrollbarSize,       // float     ScrollbarSize
    ImGuiStyleVar_ScrollbarRounding,   // float     ScrollbarRounding
    ImGuiStyleVar_GrabMinSize,         // float     GrabMinSize
    ImGuiStyleVar_GrabRounding,        // float     GrabRounding
    ImGuiStyleVar_TabRounding,         // float     TabRounding
    ImGuiStyleVar_ButtonTextAlign,     // ImVec2    ButtonTextAlign
    ImGuiStyleVar_SelectableTextAlign, // ImVec2    SelectableTextAlign
    ImGuiStyleVar_SeparatorTextBorderSize,// float  SeparatorTextBorderSize
    ImGuiStyleVar_SeparatorTextAlign,  // ImVec2    SeparatorTextAlign
    ImGuiStyleVar_SeparatorTextPadding,// ImVec2    SeparatorTextPadding
    ImGuiStyleVar_COUNT
};

struct ImGuiStyle
{
    float Alpha = 1.0f;                      // Global alpha applies to everything in Dear ImGui.
    float DisabledAlpha = 0.6f;              // Additional alpha multiplier applied by BeginDisabled(). Multiply over current value of Alpha.
    ImVec2 WindowPadding = {8,8};              // Padding within a window.
    float WindowRounding = 0.0f;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows. Large values tend to lead to variety of artifacts and are not recommended.
    float WindowBorderSize = 1.0f;           // Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2 WindowMinSize = {32,32};              // Minimum window size. This is a global setting. If you want to constrain individual windows, use SetNextWindowSizeConstraints().
    ImVec2 WindowTitleAlign = {0.0f, 0.5f};           // Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
    ImGuiDir WindowMenuButtonPosition = ImGuiDir_Left;   // Side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to ImGuiDir_Left.
    float ChildRounding = 0.0f;              // Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
    float ChildBorderSize = 1.0f;            // Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    float PopupRounding = 0.0f;              // Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
    float PopupBorderSize = 1.0f;            // Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2 FramePadding = {4,3};               // Padding within a framed rectangle (used by most widgets).
    float FrameRounding = 0.0f;              // Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
    float FrameBorderSize = 0.0f;            // Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2 ItemSpacing = {8,4};                // Horizontal and vertical spacing between widgets/lines.
    ImVec2 ItemInnerSpacing = {4,4};           // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
    ImVec2 CellPadding = {4,2};                // Padding within a table cell
    ImVec2 TouchExtraPadding = {0,0};          // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    float IndentSpacing = 21.0f;              // Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    float ColumnsMinSpacing = 6.0f;          // Minimum horizontal spacing between two columns. Preferably > (FramePadding.x + 1).
    float ScrollbarSize = 14.0f;              // Width of the vertical scrollbar, Height of the horizontal scrollbar.
    float ScrollbarRounding = 9.0f;          // Radius of grab corners for scrollbar.
    float GrabMinSize = 12.0f;                // Minimum width/height of a grab box for slider/scrollbar.
    float GrabRounding = 0.0f;               // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    float LogSliderDeadzone = 4.0f;          // The size in pixels of the dead-zone around zero on logarithmic sliders that cross zero.
    float TabRounding = 4.0f;                // Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs.
    float TabBorderSize = 0.0f;              // Thickness of border around tabs.
    float TabMinWidthForCloseButton = 0.0f;  // Minimum width for close button to appear on an unselected tab when hovered. Set to 0.0f to always show when hovering, set to FLT_MAX to never show close button unless selected.
    ImGuiDir ColorButtonPosition = ImGuiDir_Right;        // Side of the color button in the ColorEdit4 widget (left/right). Defaults to ImGuiDir_Right.
    ImVec2 ButtonTextAlign = {0.5f, 0.5f};            // Alignment of button text when button is larger than text. Defaults to (0.5f, 0.5f) (centered).
    ImVec2 SelectableTextAlign = {0.0f, 0.0f};        // Alignment of selectable text. Defaults to (0.0f, 0.0f) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line.
    float SeparatorTextBorderSize = 3.0f;    // Thickkness of border in SeparatorText()
    ImVec2 SeparatorTextAlign = {0.0f, 0.5f};         // Alignment of text within the separator. Defaults to (0.0f, 0.5f) (left aligned, center).
    ImVec2 SeparatorTextPadding = {20,3};       // Horizontal offset of text from each edge of the separator + spacing on other axis. Generally small values. .y is recommended to be == FramePadding.y.
    ImVec2 DisplayWindowPadding = {19,19};       // Window position are clamped to be visible within the display area or monitors by at least this amount. Only applies to regular windows.
    ImVec2 DisplaySafeAreaPadding = {3,3};     // If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!
    float MouseCursorScale = 1.0f;           // Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later.
    bool AntiAliasedLines = true;           // Enable anti-aliased lines/borders. Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).
    bool AntiAliasedLinesUseTex = true;     // Enable anti-aliased lines/borders using textures where possible. Require backend to render with bilinear filtering (NOT point/nearest filtering). Latched at the beginning of the frame (copied to ImDrawList).
    bool AntiAliasedFill = true;            // Enable anti-aliased edges around filled shapes (rounded rectangles, circles, etc.). Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).
    float CurveTessellationTol = 1.25f;       // Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
    float CircleTessellationMaxError = 0.3f; // Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.
    ImVec4 Colors[ImGuiCol_COUNT];

    ImGuiStyle();
    void ScaleAllSizes(float scale_factor);
};

namespace ImGui
{
    void StyleColorsDark(ImGuiStyle& style);    // new, recommended style (default)
    void StyleColorsLight(ImGuiStyle& style);   // best used with borders and a custom, thicker font
    void StyleColorsClassic(ImGuiStyle& style); // classic imgui style
}
