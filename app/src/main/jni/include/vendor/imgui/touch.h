#pragma once
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_internal.h"

static bool IsImGuiTouchPoint(float x, float y) {
    ImGuiContext* g = ImGui::GetCurrentContext();
    if (!g) return false;

    // Check if the touch point is within any ImGui window
    for (int i = 0; i < g->Windows.Size; i++) {
        ImGuiWindow* window = g->Windows[i];
        if (window->WasActive && !(window->Flags & ImGuiWindowFlags_NoInputs)) {
            if (x >= window->Pos.x && x <= (window->Pos.x + window->Size.x) &&
                y >= window->Pos.y && y <= (window->Pos.y + window->Size.y)) {
                return true;
            }
        }
    }
    return false;
}
