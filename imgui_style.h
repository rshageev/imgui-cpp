#pragma once

#include "imgui_types.h"
#include "imgui_math.h"
#include "imgui_color.h"

#include <array>

// Enumeration for PushStyleColor() / PopStyleColor()
using ImGuiCol = int;
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

using ColorScheme = std::array<ImColorf, ImGuiCol_COUNT>;

template<size_t N>
constexpr ColorScheme MakeColorScheme(std::pair<ImGuiCol, ImColorf> (&&colors)[N])
{
    ColorScheme out;
    for (auto [idx, col] : colors) { out[idx] = col; }
    return out;
}

namespace Style
{

inline constexpr auto Dark = MakeColorScheme(
{
    {ImGuiCol_Text,                  {1.00f, 1.00f, 1.00f, 1.00f}},
    {ImGuiCol_TextDisabled,          {0.50f, 0.50f, 0.50f, 1.00f}},
    {ImGuiCol_WindowBg,              {0.06f, 0.06f, 0.06f, 0.94f}},
    {ImGuiCol_ChildBg,               {0.00f, 0.00f, 0.00f, 0.00f}},
    {ImGuiCol_PopupBg,               {0.08f, 0.08f, 0.08f, 0.94f}},
    {ImGuiCol_Border,                {0.43f, 0.43f, 0.50f, 0.50f}},
    {ImGuiCol_BorderShadow,          {0.00f, 0.00f, 0.00f, 0.00f}},
    {ImGuiCol_FrameBg,               {0.16f, 0.29f, 0.48f, 0.54f}},
    {ImGuiCol_FrameBgHovered,        {0.26f, 0.59f, 0.98f, 0.40f}},
    {ImGuiCol_FrameBgActive,         {0.26f, 0.59f, 0.98f, 0.67f}},
    {ImGuiCol_TitleBg,               {0.04f, 0.04f, 0.04f, 1.00f}},
    {ImGuiCol_TitleBgActive,         {0.16f, 0.29f, 0.48f, 1.00f}},
    {ImGuiCol_TitleBgCollapsed,      {0.00f, 0.00f, 0.00f, 0.51f}},
    {ImGuiCol_MenuBarBg,             {0.14f, 0.14f, 0.14f, 1.00f}},
    {ImGuiCol_ScrollbarBg,           {0.02f, 0.02f, 0.02f, 0.53f}},
    {ImGuiCol_ScrollbarGrab,         {0.31f, 0.31f, 0.31f, 1.00f}},
    {ImGuiCol_ScrollbarGrabHovered,  {0.41f, 0.41f, 0.41f, 1.00f}},
    {ImGuiCol_ScrollbarGrabActive,   {0.51f, 0.51f, 0.51f, 1.00f}},
    {ImGuiCol_CheckMark,             {0.26f, 0.59f, 0.98f, 1.00f}},
    {ImGuiCol_SliderGrab,            {0.24f, 0.52f, 0.88f, 1.00f}},
    {ImGuiCol_SliderGrabActive,      {0.26f, 0.59f, 0.98f, 1.00f}},
    {ImGuiCol_Button,                {0.26f, 0.59f, 0.98f, 0.40f}},
    {ImGuiCol_ButtonHovered,         {0.26f, 0.59f, 0.98f, 1.00f}},
    {ImGuiCol_ButtonActive,          {0.06f, 0.53f, 0.98f, 1.00f}},
    {ImGuiCol_Header,                {0.26f, 0.59f, 0.98f, 0.31f}},
    {ImGuiCol_HeaderHovered,         {0.26f, 0.59f, 0.98f, 0.80f}},
    {ImGuiCol_HeaderActive,          {0.26f, 0.59f, 0.98f, 1.00f}},
    {ImGuiCol_Separator,             {0.43f, 0.43f, 0.50f, 0.50f}},
    {ImGuiCol_SeparatorHovered,      {0.10f, 0.40f, 0.75f, 0.78f}},
    {ImGuiCol_SeparatorActive,       {0.10f, 0.40f, 0.75f, 1.00f}},
    {ImGuiCol_ResizeGrip,            {0.26f, 0.59f, 0.98f, 0.20f}},
    {ImGuiCol_ResizeGripHovered,     {0.26f, 0.59f, 0.98f, 0.67f}},
    {ImGuiCol_ResizeGripActive,      {0.26f, 0.59f, 0.98f, 0.95f}},
    {ImGuiCol_Tab,                   {0.18f, 0.35f, 0.58f, 0.86f}},
    {ImGuiCol_TabHovered,            {0.26f, 0.59f, 0.98f, 0.80f}},
    {ImGuiCol_TabActive,             {0.20f, 0.41f, 0.68f, 1.00f}},
    {ImGuiCol_TabUnfocused,          {0.07f, 0.10f, 0.15f, 0.97f}},
    {ImGuiCol_TabUnfocusedActive,    {0.14f, 0.26f, 0.42f, 1.00f}},
    {ImGuiCol_PlotLines,             {0.61f, 0.61f, 0.61f, 1.00f}},
    {ImGuiCol_PlotLinesHovered,      {1.00f, 0.43f, 0.35f, 1.00f}},
    {ImGuiCol_PlotHistogram,         {0.90f, 0.70f, 0.00f, 1.00f}},
    {ImGuiCol_PlotHistogramHovered,  {1.00f, 0.60f, 0.00f, 1.00f}},
    {ImGuiCol_TableHeaderBg,         {0.19f, 0.19f, 0.20f, 1.00f}},
    {ImGuiCol_TableBorderStrong,     {0.31f, 0.31f, 0.35f, 1.00f}},
    {ImGuiCol_TableBorderLight,      {0.23f, 0.23f, 0.25f, 1.00f}},
    {ImGuiCol_TableRowBg,            {0.00f, 0.00f, 0.00f, 0.00f}},
    {ImGuiCol_TableRowBgAlt,         {1.00f, 1.00f, 1.00f, 0.06f}},
    {ImGuiCol_TextSelectedBg,        {0.26f, 0.59f, 0.98f, 0.35f}},
    {ImGuiCol_DragDropTarget,        {1.00f, 1.00f, 0.00f, 0.90f}},
    {ImGuiCol_NavHighlight,          {0.26f, 0.59f, 0.98f, 1.00f}},
    {ImGuiCol_NavWindowingHighlight, {1.00f, 1.00f, 1.00f, 0.70f}},
    {ImGuiCol_NavWindowingDimBg,     {0.80f, 0.80f, 0.80f, 0.20f}},
    {ImGuiCol_ModalWindowDimBg,      {0.80f, 0.80f, 0.80f, 0.35f}},
});

inline constexpr auto Classic = MakeColorScheme({
    {ImGuiCol_Text,                  {0.90f, 0.90f, 0.90f, 1.00f}},
    {ImGuiCol_TextDisabled,          {0.60f, 0.60f, 0.60f, 1.00f}},
    {ImGuiCol_WindowBg,              {0.00f, 0.00f, 0.00f, 0.85f}},
    {ImGuiCol_ChildBg,               {0.00f, 0.00f, 0.00f, 0.00f}},
    {ImGuiCol_PopupBg,               {0.11f, 0.11f, 0.14f, 0.92f}},
    {ImGuiCol_Border,                {0.50f, 0.50f, 0.50f, 0.50f}},
    {ImGuiCol_BorderShadow,          {0.00f, 0.00f, 0.00f, 0.00f}},
    {ImGuiCol_FrameBg,               {0.43f, 0.43f, 0.43f, 0.39f}},
    {ImGuiCol_FrameBgHovered,        {0.47f, 0.47f, 0.69f, 0.40f}},
    {ImGuiCol_FrameBgActive,         {0.42f, 0.41f, 0.64f, 0.69f}},
    {ImGuiCol_TitleBg,               {0.27f, 0.27f, 0.54f, 0.83f}},
    {ImGuiCol_TitleBgActive,         {0.32f, 0.32f, 0.63f, 0.87f}},
    {ImGuiCol_TitleBgCollapsed,      {0.40f, 0.40f, 0.80f, 0.20f}},
    {ImGuiCol_MenuBarBg,             {0.40f, 0.40f, 0.55f, 0.80f}},
    {ImGuiCol_ScrollbarBg,           {0.20f, 0.25f, 0.30f, 0.60f}},
    {ImGuiCol_ScrollbarGrab,         {0.40f, 0.40f, 0.80f, 0.30f}},
    {ImGuiCol_ScrollbarGrabHovered,  {0.40f, 0.40f, 0.80f, 0.40f}},
    {ImGuiCol_ScrollbarGrabActive,   {0.41f, 0.39f, 0.80f, 0.60f}},
    {ImGuiCol_CheckMark,             {0.90f, 0.90f, 0.90f, 0.50f}},
    {ImGuiCol_SliderGrab,            {1.00f, 1.00f, 1.00f, 0.30f}},
    {ImGuiCol_SliderGrabActive,      {0.41f, 0.39f, 0.80f, 0.60f}},
    {ImGuiCol_Button,                {0.35f, 0.40f, 0.61f, 0.62f}},
    {ImGuiCol_ButtonHovered,         {0.40f, 0.48f, 0.71f, 0.79f}},
    {ImGuiCol_ButtonActive,          {0.46f, 0.54f, 0.80f, 1.00f}},
    {ImGuiCol_Header,                {0.40f, 0.40f, 0.90f, 0.45f}},
    {ImGuiCol_HeaderHovered,         {0.45f, 0.45f, 0.90f, 0.80f}},
    {ImGuiCol_HeaderActive,          {0.53f, 0.53f, 0.87f, 0.80f}},
    {ImGuiCol_Separator,             {0.50f, 0.50f, 0.50f, 0.60f}},
    {ImGuiCol_SeparatorHovered,      {0.60f, 0.60f, 0.70f, 1.00f}},
    {ImGuiCol_SeparatorActive,       {0.70f, 0.70f, 0.90f, 1.00f}},
    {ImGuiCol_ResizeGrip,            {1.00f, 1.00f, 1.00f, 0.10f}},
    {ImGuiCol_ResizeGripHovered,     {0.78f, 0.82f, 1.00f, 0.60f}},
    {ImGuiCol_ResizeGripActive,      {0.78f, 0.82f, 1.00f, 0.90f}},
    {ImGuiCol_Tab,                   {0.34f, 0.34f, 0.68f, 0.79f}},
    {ImGuiCol_TabHovered,            {0.45f, 0.45f, 0.90f, 0.80f}},
    {ImGuiCol_TabActive,             {0.40f, 0.40f, 0.73f, 0.84f}},
    {ImGuiCol_TabUnfocused,          {0.28f, 0.28f, 0.57f, 0.82f}},
    {ImGuiCol_TabUnfocusedActive,    {0.35f, 0.35f, 0.65f, 0.84f}},
    {ImGuiCol_PlotLines,             {1.00f, 1.00f, 1.00f, 1.00f}},
    {ImGuiCol_PlotLinesHovered,      {0.90f, 0.70f, 0.00f, 1.00f}},
    {ImGuiCol_PlotHistogram,         {0.90f, 0.70f, 0.00f, 1.00f}},
    {ImGuiCol_PlotHistogramHovered,  {1.00f, 0.60f, 0.00f, 1.00f}},
    {ImGuiCol_TableHeaderBg,         {0.27f, 0.27f, 0.38f, 1.00f}},
    {ImGuiCol_TableBorderStrong,     {0.31f, 0.31f, 0.45f, 1.00f}},
    {ImGuiCol_TableBorderLight,      {0.26f, 0.26f, 0.28f, 1.00f}},
    {ImGuiCol_TableRowBg,            {0.00f, 0.00f, 0.00f, 0.00f}},
    {ImGuiCol_TableRowBgAlt,         {1.00f, 1.00f, 1.00f, 0.07f}},
    {ImGuiCol_TextSelectedBg,        {0.00f, 0.00f, 1.00f, 0.35f}},
    {ImGuiCol_DragDropTarget,        {1.00f, 1.00f, 0.00f, 0.90f}},
    {ImGuiCol_NavHighlight,          {0.45f, 0.45f, 0.90f, 0.80f}},
    {ImGuiCol_NavWindowingHighlight, {1.00f, 1.00f, 1.00f, 0.70f}},
    {ImGuiCol_NavWindowingDimBg,     {0.80f, 0.80f, 0.80f, 0.20f}},
    {ImGuiCol_ModalWindowDimBg,      {0.20f, 0.20f, 0.20f, 0.35f}},
});

inline constexpr auto Light = MakeColorScheme({
    {ImGuiCol_Text,                  {0.00f, 0.00f, 0.00f, 1.00f}},
    {ImGuiCol_TextDisabled,          {0.60f, 0.60f, 0.60f, 1.00f}},
    {ImGuiCol_WindowBg,              {0.94f, 0.94f, 0.94f, 1.00f}},
    {ImGuiCol_ChildBg,               {0.00f, 0.00f, 0.00f, 0.00f}},
    {ImGuiCol_PopupBg,               {1.00f, 1.00f, 1.00f, 0.98f}},
    {ImGuiCol_Border,                {0.00f, 0.00f, 0.00f, 0.30f}},
    {ImGuiCol_BorderShadow,          {0.00f, 0.00f, 0.00f, 0.00f}},
    {ImGuiCol_FrameBg,               {1.00f, 1.00f, 1.00f, 1.00f}},
    {ImGuiCol_FrameBgHovered,        {0.26f, 0.59f, 0.98f, 0.40f}},
    {ImGuiCol_FrameBgActive,         {0.26f, 0.59f, 0.98f, 0.67f}},
    {ImGuiCol_TitleBg,               {0.96f, 0.96f, 0.96f, 1.00f}},
    {ImGuiCol_TitleBgActive,         {0.82f, 0.82f, 0.82f, 1.00f}},
    {ImGuiCol_TitleBgCollapsed,      {1.00f, 1.00f, 1.00f, 0.51f}},
    {ImGuiCol_MenuBarBg,             {0.86f, 0.86f, 0.86f, 1.00f}},
    {ImGuiCol_ScrollbarBg,           {0.98f, 0.98f, 0.98f, 0.53f}},
    {ImGuiCol_ScrollbarGrab,         {0.69f, 0.69f, 0.69f, 0.80f}},
    {ImGuiCol_ScrollbarGrabHovered,  {0.49f, 0.49f, 0.49f, 0.80f}},
    {ImGuiCol_ScrollbarGrabActive,   {0.49f, 0.49f, 0.49f, 1.00f}},
    {ImGuiCol_CheckMark,             {0.26f, 0.59f, 0.98f, 1.00f}},
    {ImGuiCol_SliderGrab,            {0.26f, 0.59f, 0.98f, 0.78f}},
    {ImGuiCol_SliderGrabActive,      {0.46f, 0.54f, 0.80f, 0.60f}},
    {ImGuiCol_Button,                {0.26f, 0.59f, 0.98f, 0.40f}},
    {ImGuiCol_ButtonHovered,         {0.26f, 0.59f, 0.98f, 1.00f}},
    {ImGuiCol_ButtonActive,          {0.06f, 0.53f, 0.98f, 1.00f}},
    {ImGuiCol_Header,                {0.26f, 0.59f, 0.98f, 0.31f}},
    {ImGuiCol_HeaderHovered,         {0.26f, 0.59f, 0.98f, 0.80f}},
    {ImGuiCol_HeaderActive,          {0.26f, 0.59f, 0.98f, 1.00f}},
    {ImGuiCol_Separator,             {0.39f, 0.39f, 0.39f, 0.62f}},
    {ImGuiCol_SeparatorHovered,      {0.14f, 0.44f, 0.80f, 0.78f}},
    {ImGuiCol_SeparatorActive,       {0.14f, 0.44f, 0.80f, 1.00f}},
    {ImGuiCol_ResizeGrip,            {0.35f, 0.35f, 0.35f, 0.17f}},
    {ImGuiCol_ResizeGripHovered,     {0.26f, 0.59f, 0.98f, 0.67f}},
    {ImGuiCol_ResizeGripActive,      {0.26f, 0.59f, 0.98f, 0.95f}},
    {ImGuiCol_Tab,                   {0.76f, 0.80f, 0.84f, 0.93f}},
    {ImGuiCol_TabHovered,            {0.26f, 0.59f, 0.98f, 0.80f}},
    {ImGuiCol_TabActive,             {0.60f, 0.73f, 0.88f, 1.00f}},
    {ImGuiCol_TabUnfocused,          {0.92f, 0.93f, 0.94f, 0.99f}},
    {ImGuiCol_TabUnfocusedActive,    {0.74f, 0.82f, 0.91f, 1.00f}},
    {ImGuiCol_PlotLines,             {0.39f, 0.39f, 0.39f, 1.00f}},
    {ImGuiCol_PlotLinesHovered,      {1.00f, 0.43f, 0.35f, 1.00f}},
    {ImGuiCol_PlotHistogram,         {0.90f, 0.70f, 0.00f, 1.00f}},
    {ImGuiCol_PlotHistogramHovered,  {1.00f, 0.45f, 0.00f, 1.00f}},
    {ImGuiCol_TableHeaderBg,         {0.78f, 0.87f, 0.98f, 1.00f}},
    {ImGuiCol_TableBorderStrong,     {0.57f, 0.57f, 0.64f, 1.00f}},
    {ImGuiCol_TableBorderLight,      {0.68f, 0.68f, 0.74f, 1.00f}},
    {ImGuiCol_TableRowBg,            {0.00f, 0.00f, 0.00f, 0.00f}},
    {ImGuiCol_TableRowBgAlt,         {0.30f, 0.30f, 0.30f, 0.09f}},
    {ImGuiCol_TextSelectedBg,        {0.26f, 0.59f, 0.98f, 0.35f}},
    {ImGuiCol_DragDropTarget,        {0.26f, 0.59f, 0.98f, 0.95f}},
    {ImGuiCol_NavHighlight,          {0.26f, 0.59f, 0.98f, 0.80f}},
    {ImGuiCol_NavWindowingHighlight, {0.70f, 0.70f, 0.70f, 0.70f}},
    {ImGuiCol_NavWindowingDimBg,     {0.20f, 0.20f, 0.20f, 0.20f}},
    {ImGuiCol_ModalWindowDimBg,      {0.20f, 0.20f, 0.20f, 0.35f}},
});

}



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
    ImVec2 WindowPadding = {8,8};            // Padding within a window.
    float WindowRounding = 0.0f;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows. Large values tend to lead to variety of artifacts and are not recommended.
    float WindowBorderSize = 1.0f;           // Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2 WindowMinSize = {32,32};          // Minimum window size. This is a global setting. If you want to constrain individual windows, use SetNextWindowSizeConstraints().
    ImVec2 WindowTitleAlign = {0.0f, 0.5f};  // Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
    ImGuiDir WindowMenuButtonPosition = ImGuiDir_Left; // Side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to ImGuiDir_Left.
    float ChildRounding = 0.0f;              // Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
    float ChildBorderSize = 1.0f;            // Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    float PopupRounding = 0.0f;              // Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
    float PopupBorderSize = 1.0f;            // Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2 FramePadding = {4,3};             // Padding within a framed rectangle (used by most widgets).
    float FrameRounding = 0.0f;              // Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
    float FrameBorderSize = 0.0f;            // Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2 ItemSpacing = {8,4};              // Horizontal and vertical spacing between widgets/lines.
    ImVec2 ItemInnerSpacing = {4,4};         // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
    ImVec2 CellPadding = {4,2};              // Padding within a table cell
    ImVec2 TouchExtraPadding = {0,0};        // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    float IndentSpacing = 21.0f;             // Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    float ScrollbarSize = 14.0f;             // Width of the vertical scrollbar, Height of the horizontal scrollbar.
    float ScrollbarRounding = 9.0f;          // Radius of grab corners for scrollbar.
    float GrabMinSize = 12.0f;               // Minimum width/height of a grab box for slider/scrollbar.
    float GrabRounding = 0.0f;               // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    float LogSliderDeadzone = 4.0f;          // The size in pixels of the dead-zone around zero on logarithmic sliders that cross zero.
    float TabRounding = 4.0f;                // Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs.
    float TabBorderSize = 0.0f;              // Thickness of border around tabs.
    float TabMinWidthForCloseButton = 0.0f;  // Minimum width for close button to appear on an unselected tab when hovered. Set to 0.0f to always show when hovering, set to FLT_MAX to never show close button unless selected.
    ImGuiDir ColorButtonPosition = ImGuiDir_Right; // Side of the color button in the ColorEdit4 widget (left/right). Defaults to ImGuiDir_Right.
    ImVec2 ButtonTextAlign = {0.5f, 0.5f};   // Alignment of button text when button is larger than text. Defaults to (0.5f, 0.5f) (centered).
    ImVec2 SelectableTextAlign = {0.0f, 0.0f}; // Alignment of selectable text. Defaults to (0.0f, 0.0f) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line.
    float SeparatorTextBorderSize = 3.0f;    // Thickkness of border in SeparatorText()
    ImVec2 SeparatorTextAlign = {0.0f, 0.5f}; // Alignment of text within the separator. Defaults to (0.0f, 0.5f) (left aligned, center).
    ImVec2 SeparatorTextPadding = {20,3};    // Horizontal offset of text from each edge of the separator + spacing on other axis. Generally small values. .y is recommended to be == FramePadding.y.
    ImVec2 DisplayWindowPadding = {19,19};   // Window position are clamped to be visible within the display area or monitors by at least this amount. Only applies to regular windows.
    ImVec2 DisplaySafeAreaPadding = {3,3};   // If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!
    float MouseCursorScale = 1.0f;           // Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later.
    bool AntiAliasedLines = true;            // Enable anti-aliased lines/borders. Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).
    bool AntiAliasedLinesUseTex = true;      // Enable anti-aliased lines/borders using textures where possible. Require backend to render with bilinear filtering (NOT point/nearest filtering). Latched at the beginning of the frame (copied to ImDrawList).
    bool AntiAliasedFill = true;             // Enable anti-aliased edges around filled shapes (rounded rectangles, circles, etc.). Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).
    float CurveTessellationTol = 1.25f;      // Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
    float CircleTessellationMaxError = 0.3f; // Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.
    ColorScheme Colors = Style::Dark;

    void ScaleAllSizes(float scale_factor);
};
