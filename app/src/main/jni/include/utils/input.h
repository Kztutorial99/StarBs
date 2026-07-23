#pragma once

#include "vendor/And64InlineHook/And64InlineHook.hpp"
#include "vendor/xdl/xdl.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/touch.h"
#include "utils/dlfcn.h"
#include "vendor/imgui/backends/imgui_impl_android.h"
#include "utils/obfuscate.h"
#include "utils/features.h"
#include "vendor/Oxorany/oxorany.h"
#include "utils/xorstr.hpp"
#include <android/input.h>
#include <dlfcn.h>

bool g_DraggingFromOutsideImGui = false;
int g_ActivePointerIndex = -1;

bool IsSupportedAction(int32_t actionMasked) {
    switch (actionMasked) {
        case AMOTION_EVENT_ACTION_DOWN:
        case AMOTION_EVENT_ACTION_UP:
        case AMOTION_EVENT_ACTION_MOVE:
        case AMOTION_EVENT_ACTION_CANCEL:
        case AMOTION_EVENT_ACTION_POINTER_DOWN:
        case AMOTION_EVENT_ACTION_POINTER_UP:
        case AMOTION_EVENT_ACTION_HOVER_MOVE:
        case AMOTION_EVENT_ACTION_SCROLL:
            return true;
        default:
            return false;
    }
}

float (*orig_AMotionEvent_getX)(AInputEvent* motion_event, size_t pointer_index) = nullptr;
float (*orig_AMotionEvent_getY)(AInputEvent* motion_event, size_t pointer_index) = nullptr;

float hooked_AMotionEvent_getX(AInputEvent* motion_event, size_t pointer_index) {
    if (!_features[xorstr_("bBetterTouch")] || !orig_AMotionEvent_getY || !orig_AMotionEvent_getX)
        return orig_AMotionEvent_getX(motion_event, pointer_index);

    float x = orig_AMotionEvent_getX(motion_event, pointer_index);
    float y = orig_AMotionEvent_getY(motion_event, pointer_index);

    int32_t action = AMotionEvent_getAction(motion_event);
    int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;

    if (!IsSupportedAction(actionMasked)) return x;

    if (actionMasked == AMOTION_EVENT_ACTION_DOWN || actionMasked == AMOTION_EVENT_ACTION_POINTER_DOWN) {
        if (!IsImGuiTouchPoint(x, y)) {
            g_DraggingFromOutsideImGui = true;
            g_ActivePointerIndex = pointer_index;
        } else g_DraggingFromOutsideImGui = false;
    } else if (actionMasked == AMOTION_EVENT_ACTION_UP ||
               actionMasked == AMOTION_EVENT_ACTION_POINTER_UP ||
               actionMasked == AMOTION_EVENT_ACTION_CANCEL) {
        if (pointer_index == g_ActivePointerIndex) {
            g_DraggingFromOutsideImGui = false;
            g_ActivePointerIndex = -1;
        }
    }

    if (g_DraggingFromOutsideImGui && (int)pointer_index == g_ActivePointerIndex)
        return x;

    return IsImGuiTouchPoint(x, y) ? -99999.0f : x;
}

float hooked_AMotionEvent_getY(AInputEvent* motion_event, size_t pointer_index) {
    if (!_features[xorstr_("bBetterTouch")] || !orig_AMotionEvent_getX || !orig_AMotionEvent_getY)
        return orig_AMotionEvent_getY(motion_event, pointer_index);

    float y = orig_AMotionEvent_getY(motion_event, pointer_index);
    float x = orig_AMotionEvent_getX(motion_event, pointer_index);

    if (g_DraggingFromOutsideImGui && (int)pointer_index == g_ActivePointerIndex)
        return y;

    return IsImGuiTouchPoint(x, y) ? -99999.0f : y;
}

void (*orig_InitializeMotionEvent)(AInputEvent* event, const void* msg) = nullptr;
void hooked_InitializeMotionEvent(AInputEvent* event, const void* msg) {
    if (orig_InitializeMotionEvent) orig_InitializeMotionEvent(event, msg);
    if (!event) return;

    ImGui_ImplAndroid_HandleInputEvent(event, {0.0f, 0.0f});
}

int32_t (*origConsume)(void *thiz, void *arg1, bool arg2, long arg3, uint32_t *arg4, AInputEvent **input_event) = nullptr;
void (*origInput)(void *thiz, void *ex_ab, void *ex_ac) = nullptr;

int32_t myConsume(void *thiz, void *arg1, bool arg2, long arg3, uint32_t *arg4, AInputEvent **input_event) {
    int32_t result = origConsume(thiz, arg1, arg2, arg3, arg4, input_event);
    if (result != 0 || *input_event == nullptr)
        return result;
    ImGui_ImplAndroid_HandleInputEvent(*input_event, ImVec2(1.0f, 1.0f));
    return result;
}

void myInput(void *thiz, void *ex_ab, void *ex_ac) {
    if (origInput) origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent*)thiz, ImVec2(1.0f, 1.0f));
}

void __INPUT__() {
    void *handle_input = xdl_open(xorstr_("libinput.so"), XDL_DEFAULT);

    void *xdl_sym_input = xdl_sym(handle_input,
                                  xorstr_("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"),
                                  nullptr);

    if (xdl_sym_input == nullptr) {
        xdl_sym_input = xdl_sym(handle_input,
                                xorstr_("_ZN7android13InputConsumer7consumeEPNS_26InputEventFactoryInterfaceEblPjPPNS_10InputEventE"),
                                nullptr);
        if (xdl_sym_input) DobbyHook(xdl_sym_input, (void*)myConsume, (void**)&origConsume);
    } else {
        DobbyHook(xdl_sym_input, (void*)myInput, (void**)&origInput);
    }

    void *libc = xdl_open(xorstr_("libc.so"), XDL_DEFAULT);
    if (libc) {
        void* getX = xdl_sym(libc, xorstr_("AMotionEvent_getX"), nullptr);
        void* getY = xdl_sym(libc, xorstr_("AMotionEvent_getY"), nullptr);
        if (getX) DobbyHook(getX, (void*)hooked_AMotionEvent_getX, (void**)&orig_AMotionEvent_getX);
        if (getY) DobbyHook(getY, (void*)hooked_AMotionEvent_getY, (void**)&orig_AMotionEvent_getY);
    }
}
