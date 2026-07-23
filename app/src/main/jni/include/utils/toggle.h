#pragma once
#include "imgui/imgui.h"

bool Toggle(const char* str_id, bool* v)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Sizing: Ultra-compact professional style
    float height = 25.0f;
    float width = 48.0f;
    float radius = height * 0.5f;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    bool clicked = ImGui::IsItemClicked();
    if (clicked)
        *v = !*v;

    // Aesthetic: Professional Deep Charcoal and Slate Lavender
    ImU32 col_bg = *v ? IM_COL32(110, 100, 240, 255) : IM_COL32(40, 40, 45, 255);
    ImU32 col_border = *v ? IM_COL32(130, 120, 255, 255) : IM_COL32(70, 70, 80, 255);

    // Render Track
    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, radius);
    draw_list->AddRect(p, ImVec2(p.x + width, p.y + height), col_border, radius, 0, 1.5f);

    // Knob Styling
    float knob_offset = 3.5f;
    float knob_radius = radius - knob_offset;
    float circle_x = *v ? (p.x + width - radius) : (p.x + radius);

    // Knob Glow
    if (*v) {
        draw_list->AddCircleFilled(ImVec2(circle_x, p.y + radius), knob_radius + 3.0f, IM_COL32(110, 100, 240, 60));
    }

    // Render Knob
    draw_list->AddCircleFilled(ImVec2(circle_x, p.y + radius), knob_radius, *v ? IM_COL32(255, 255, 255, 255) : IM_COL32(180, 180, 190, 255));

    // Label Positioning
    ImGui::SameLine(0, 15);
    float text_y_offset = (height - ImGui::GetFontSize()) / 2.0f;
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + text_y_offset);
    ImGui::Text("%s", str_id);

    return clicked;
}
