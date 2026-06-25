#pragma once

#include <imgui.h>

static const ImColor ColorBlue = ImColor{2, 85, 194, 255};
static const ImColor ColorPink = ImColor{212, 38, 151, 255};
static const ImColor ColorYellow = ImColor{247, 170, 2, 255};
static const ImColor ColorGreen = ImColor{3, 171, 68, 255};
static const ImColor ColorViolet = ImColor{168, 7, 222, 255};

inline void style_neon_dark()
{
    ImGuiStyle& s = ImGui::GetStyle();

    s.WindowPadding = {14.0f, 14.0f};
    s.FramePadding = {8.0f, 5.0f};
    s.ItemSpacing = {8.0f, 6.0f};
    s.ItemInnerSpacing = {6.0f, 4.0f};
    s.ScrollbarSize = 12.0f;
    s.GrabMinSize = 20.0f;

    s.WindowRounding = 6.0f;
    s.ChildRounding = 4.0f;
    s.FrameRounding = 4.0f;
    s.PopupRounding = 4.0f;
    s.ScrollbarRounding = 2.0f;
    s.GrabRounding = 4.0f;
    s.TabRounding = 4.0f;

    s.WindowBorderSize = 1.0f;
    s.ChildBorderSize = 1.0f;
    s.FrameBorderSize = 1.0f;
    s.PopupBorderSize = 1.0f;

    ImVec4* c = s.Colors;

    c[ImGuiCol_WindowBg] = ImVec4{0.039f, 0.047f, 0.063f, 0.97f};
    c[ImGuiCol_ChildBg] = ImVec4{0.051f, 0.063f, 0.082f, 1.00f};
    c[ImGuiCol_PopupBg] = ImVec4{0.043f, 0.051f, 0.071f, 0.98f};

    c[ImGuiCol_Border] = ImVec4{0.00f, 0.75f, 1.00f, 0.22f};
    c[ImGuiCol_BorderShadow] = ImVec4{0.00f, 0.00f, 0.00f, 0.50f};

    c[ImGuiCol_FrameBg] = ImVec4{0.07f, 0.09f, 0.13f, 1.00f};
    c[ImGuiCol_FrameBgHovered] = ImVec4{0.09f, 0.14f, 0.22f, 1.00f};
    c[ImGuiCol_FrameBgActive] = ImVec4{0.10f, 0.20f, 0.32f, 1.00f};

    c[ImGuiCol_TitleBg] = ImVec4{0.03f, 0.04f, 0.06f, 1.00f};
    c[ImGuiCol_TitleBgActive] = ImVec4{0.04f, 0.09f, 0.16f, 1.00f};
    c[ImGuiCol_TitleBgCollapsed] = ImVec4{0.03f, 0.04f, 0.06f, 0.80f};

    c[ImGuiCol_MenuBarBg] = ImVec4{0.04f, 0.07f, 0.12f, 1.00f};

    c[ImGuiCol_ScrollbarBg] = ImVec4{0.03f, 0.04f, 0.06f, 0.90f};
    c[ImGuiCol_ScrollbarGrab] = ImVec4{0.00f, 0.50f, 0.70f, 0.60f};
    c[ImGuiCol_ScrollbarGrabHovered] = ImVec4{0.00f, 0.65f, 0.90f, 0.80f};
    c[ImGuiCol_ScrollbarGrabActive] = ImVec4{0.00f, 0.78f, 1.00f, 1.00f};

    c[ImGuiCol_CheckMark] = ImVec4{0.00f, 0.95f, 0.85f, 1.00f};
    c[ImGuiCol_SliderGrab] = ImVec4{0.00f, 0.78f, 1.00f, 0.90f};
    c[ImGuiCol_SliderGrabActive] = ImVec4{0.00f, 0.95f, 1.00f, 1.00f};

    c[ImGuiCol_Button] = ImVec4{0.06f, 0.13f, 0.22f, 1.00f};
    c[ImGuiCol_ButtonHovered] = ImVec4{0.07f, 0.22f, 0.38f, 1.00f};
    c[ImGuiCol_ButtonActive] = ImVec4{0.00f, 0.45f, 0.70f, 1.00f};

    c[ImGuiCol_Header] = ImVec4{0.07f, 0.16f, 0.28f, 1.00f};
    c[ImGuiCol_HeaderHovered] = ImVec4{0.08f, 0.24f, 0.40f, 1.00f};
    c[ImGuiCol_HeaderActive] = ImVec4{0.00f, 0.42f, 0.65f, 1.00f};

    c[ImGuiCol_Separator] = ImVec4{0.00f, 0.60f, 0.85f, 0.30f};
    c[ImGuiCol_SeparatorHovered] = ImVec4{0.00f, 0.75f, 1.00f, 0.55f};
    c[ImGuiCol_SeparatorActive] = ImVec4{0.00f, 0.88f, 1.00f, 0.80f};

    c[ImGuiCol_ResizeGrip] = ImVec4{0.00f, 0.60f, 0.85f, 0.25f};
    c[ImGuiCol_ResizeGripHovered] = ImVec4{0.00f, 0.75f, 1.00f, 0.55f};
    c[ImGuiCol_ResizeGripActive] = ImVec4{0.00f, 0.88f, 1.00f, 0.80f};

    // Tabs
    c[ImGuiCol_Tab] = ImVec4{0.05f, 0.10f, 0.18f, 1.00f};
    c[ImGuiCol_TabHovered] = ImVec4{0.07f, 0.22f, 0.38f, 1.00f};
    c[ImGuiCol_TabActive] = ImVec4{0.05f, 0.18f, 0.30f, 1.00f};
    c[ImGuiCol_TabUnfocused] = ImVec4{0.04f, 0.08f, 0.14f, 1.00f};
    c[ImGuiCol_TabUnfocusedActive] = ImVec4{0.05f, 0.13f, 0.22f, 1.00f};

    c[ImGuiCol_PlotLines] = ImVec4{0.00f, 0.95f, 0.85f, 1.00f};
    c[ImGuiCol_PlotLinesHovered] = ImVec4{0.00f, 1.00f, 0.78f, 1.00f};
    c[ImGuiCol_PlotHistogram] = ImVec4{0.00f, 0.78f, 1.00f, 1.00f};
    c[ImGuiCol_PlotHistogramHovered] = ImVec4{0.00f, 0.92f, 1.00f, 1.00f};

    c[ImGuiCol_TableHeaderBg] = ImVec4{0.05f, 0.10f, 0.18f, 1.00f};
    c[ImGuiCol_TableBorderStrong] = ImVec4{0.00f, 0.55f, 0.80f, 0.70f};
    c[ImGuiCol_TableBorderLight] = ImVec4{0.00f, 0.35f, 0.55f, 0.40f};
    c[ImGuiCol_TableRowBg] = ImVec4{0.00f, 0.00f, 0.00f, 0.00f};
    c[ImGuiCol_TableRowBgAlt] = ImVec4{1.00f, 1.00f, 1.00f, 0.035f};

    c[ImGuiCol_TextSelectedBg] = ImVec4{0.00f, 0.55f, 0.80f, 0.40f};
    c[ImGuiCol_DragDropTarget] = ImVec4{0.00f, 0.90f, 1.00f, 0.90f};
    c[ImGuiCol_NavHighlight] = ImVec4{0.00f, 0.80f, 1.00f, 1.00f};
    c[ImGuiCol_NavWindowingHighlight] = ImVec4{0.00f, 0.80f, 1.00f, 0.70f};
    c[ImGuiCol_NavWindowingDimBg] = ImVec4{0.00f, 0.00f, 0.00f, 0.65f};
    c[ImGuiCol_ModalWindowDimBg] = ImVec4{0.00f, 0.00f, 0.00f, 0.75f};

    c[ImGuiCol_Text] = ImVec4{0.82f, 0.92f, 1.00f, 1.00f};
    c[ImGuiCol_TextDisabled] = ImVec4{0.28f, 0.38f, 0.50f, 1.00f};
}
