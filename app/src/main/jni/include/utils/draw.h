#define IMGUI_DEFINE_MATH_OPERATORS
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_internal.h"
#include "vendor/imgui/backends/imgui_impl_android.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"
#include <EGL/egl.h>
#include <string>
#include <atomic>
#include <unistd.h>

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "com.example.app" // Fallback package name
#endif

int Width, Height;
using namespace ImGui;
using namespace std;
#include "utils/xorstr.hpp"
const char* INI_PATH = xorstr_("/data/data/" PACKAGE_NAME "/imgui/.ini");
#include <vector>
#include <jni.h>
#include <android/native_activity.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <android/input.h>
#include "utils/json.hpp"

#include <GLES2/gl2.h>
#include "assets/fonts/room_info_font.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include "imgui/DreiFont.h"
#include "utils/obfuscate.h"
#include "utils/dlfcn.h"
// #include "include/hpp/imgui/persistence.h" // File tidak ditemukan, di-comment sementara
#include "vendor/imgui/touch.h"
#include "app/sync_with_py.h"
#include "app/sync_entities.h"
#include "utils/helpers.h"
#include "app/variables.h"
#include "utils/rgb.h"
#include "utils/toggle.h"
#include "imgui/stb_image.h"
#include "app/starcool.h"
#include "hooks/chams.h"
#include "jni/jni_stuff_alt.h"

static std::map<std::string, ImU32> colors = {
    {"cESP_Skeleton", IM_COL32(255, 255, 255, 255)},
    {"cESP_SkeletonHidden", IM_COL32(255, 255, 255, 120)},
    {"cESP_Line", IM_COL32(255, 0, 0, 255)},
    {"cESP_LineHidden", IM_COL32(255, 0, 0, 120)},
    {"cESP_LineBots", IM_COL32(0, 255, 0, 255)},
    {"cESP_LineBotsHidden", IM_COL32(0, 255, 0, 120)},
    {"cESP_Box", IM_COL32(255, 255, 0, 255)},
    {"cESP_BoxHidden", IM_COL32(255, 255, 0, 120)}
};

static void ClearImGuiWindowTracking() {}

void UpdateRainbow() {
    if (enableRainbow || enableRainbowWall) {
        performRGBChange();
        visibleColor = ImVec4(redd / 255.0f, greenn / 255.0f, bluee / 255.0f, 1.0f);
        if (enableRainbowWall) {
             inWallColor = ImVec4(redd / 255.0f, greenn / 255.0f, bluee / 255.0f, 1.0f);
        }
    }
}
#include "imgui/DreiFont.h"
#include "imgui/imgui_additional.h"
#include "Oxorany/oxorany.h"
#include "imgui/Icon.h"
#include "utils/xorstr.hpp"
#include "imgui/Iconcpp.h"
#include "imgui/SabFont.h"
#include "assets/raw_fonts/Custom.h"
#include "assets/raw_fonts/flamee.h"
#include "assets/raw_fonts/Japan.h"
#include "assets/raw_fonts/lexend_bold.h"
#include "assets/raw_fonts/Pubg_Font.h"
#include "assets/raw_fonts/ubuntu_medium.h"
#include "imgui/font_awesome.h"
#include "assets/fonts/icons_font_awesome_7.h"

#include <ctime>
#include <cstdlib>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdio>
#endif
#define ARGB(a, r, g, b) r << 0 | g << 8 | b << 16 | a << 24
#define ReadPointer(type, address) (*(type*)(address))
#define GetFunction(type, address, ...) ((type(*)(__VA_ARGS__))(address))
bool Zaijan = true;
#define CREATE_COLOR(r, g, b, a) new float[4]{(float)r, (float)g, (float)b, (float)a};

static GLuint menuIconTexture = 0;
static int iconW = 0, iconH = 0;

bool showMenu = false;
ImFont *flamee = nullptr;
ImFont *Japan = nullptr;
ImFont *font1 = nullptr;
ImFont *font2 = nullptr;
ImFont *ubu_0 = nullptr;
ImFont *pubg_font = nullptr;
ImFont* verdana;
namespace font {
    inline ImFont* lexend_bold;
    inline ImFont* lexend_regular;
    inline ImFont* icomoon;
    inline ImFont* icomoon_widget;
    inline ImFont* icomoon_widget2;
	inline ImFont* lexend_general_bold;
    inline ImFont* icomoon_tabs;
}

GLuint LoadTextureFromArray(unsigned char* data, int size, int* out_width, int* out_height) {
    int channels;
    unsigned char* image_data = stbi_load_from_memory(data, size, out_width, out_height, &channels, 4);
    if (!image_data) return 0;
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *out_width, *out_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(image_data);
    return tex;
}

struct My_Patches {
    MemoryPatch Recoil,Magic,Assist,Smooth;
} hexPatches;
static bool recoil = false;
static bool magic = false;
static bool assist = false;
static bool smooth = false;
static bool isPlayerBotEnabled = false;

void noMore(ImDrawList* draw) {
    if (!sync_bool["bESP"]) return;
    auto all_entities = get_entities();
    std::vector<Entity> valid_entities = all_entities;
    std::sort(valid_entities.begin(), valid_entities.end(),
        [](const Entity& a, const Entity& b) {
            float za = a.bones.count("head") ? a.bones.at("head")[2] : 0.0f;
            float zb = b.bones.count("head") ? b.bones.at("head")[2] : 0.0f;
            return za < zb;
        }
    );
    int max_entities = 20;
    if (valid_entities.size() > max_entities)
        valid_entities.resize(max_entities);

    int players = 0, bots = 0;
    for (const Entity& e : valid_entities) {
        if (!e.bones.count("head") || !e.bones.count("spine")) continue;
        if (e.is_bot) bots++; else players++;
        ImVec2 From;
        switch (sync_int["iESP_Point"]) {
            case 0: From = ImVec2(Width / 2, Height); break;
            case 1: From = ImVec2(Width / 2, Height / 2); break;
            case 2: From = ImVec2(Width / 2, 0); break;
        }
        if (sync_bool["bESP_Skeleton"]) {
            auto head = ToImVec2(e.bones.at("head"));
            auto spine = ToImVec2(e.bones.at("spine"));
            auto limbs_r_upperarm = ToImVec2(e.bones.at("limbs_r_upperarm"));
            auto limbs_r_forearm = ToImVec2(e.bones.at("limbs_r_forearm"));
            auto limbs_r_hand = ToImVec2(e.bones.at("limbs_r_hand"));
            auto limbs_r_thigh = ToImVec2(e.bones.at("limbs_r_thigh"));
            auto limbs_r_calf = ToImVec2(e.bones.at("limbs_r_calf"));
            auto limbs_r_foot = ToImVec2(e.bones.at("limbs_r_foot"));
            auto limbs_l_upperarm = ToImVec2(e.bones.at("limbs_l_upperarm"));
            auto limbs_l_forearm = ToImVec2(e.bones.at("limbs_l_forearm"));
            auto limbs_l_hand = ToImVec2(e.bones.at("limbs_l_hand"));
            auto limbs_l_thigh = ToImVec2(e.bones.at("limbs_l_thigh"));
            auto limbs_l_calf = ToImVec2(e.bones.at("limbs_l_calf"));
            auto limbs_l_foot = ToImVec2(e.bones.at("limbs_l_foot"));
            auto limbs_r_toe = ToImVec2(e.bones.at("limbs_r_toe"));
            auto limbs_l_toe = ToImVec2(e.bones.at("limbs_l_toe"));
            auto chest = ImVec2((limbs_r_upperarm.x + limbs_l_upperarm.x) / 2, (limbs_r_upperarm.y + limbs_l_upperarm.y) / 2);
            auto linecolor = e.is_visible ? colors["cESP_Skeleton"] : colors["cESP_SkeletonHidden"];
            auto thickness = sync_float["fESP_LineThickness"];
            draw->AddLine(head, chest, linecolor, thickness);
            draw->AddLine(chest, spine, linecolor, thickness);
            draw->AddLine(chest, limbs_r_upperarm, linecolor, thickness);
            draw->AddLine(chest, limbs_l_upperarm, linecolor, thickness);
            draw->AddLine(limbs_r_upperarm, limbs_r_forearm, linecolor, thickness);
            draw->AddLine(limbs_l_upperarm, limbs_l_forearm, linecolor, thickness);
            draw->AddLine(limbs_r_forearm, limbs_r_hand, linecolor, thickness);
            draw->AddLine(limbs_l_forearm, limbs_l_hand, linecolor, thickness);
            draw->AddLine(spine, limbs_l_thigh, linecolor, thickness);
            draw->AddLine(spine, limbs_r_thigh, linecolor, thickness);
            draw->AddLine(limbs_l_thigh, limbs_l_calf, linecolor, thickness);
            draw->AddLine(limbs_r_thigh, limbs_r_calf, linecolor, thickness);
            draw->AddLine(limbs_r_calf, limbs_r_foot, linecolor, thickness);
            draw->AddLine(limbs_l_calf, limbs_l_foot, linecolor, thickness);
            draw->AddLine(limbs_r_foot, limbs_r_toe, linecolor, thickness);
            draw->AddLine(limbs_l_foot, limbs_l_toe, linecolor, thickness);
        }
        ImVec2 To = ToImVec2(e.bones.at("head"));
        if (e.is_bot) {
            if (sync_bool["bESP_LineBots"]) draw->AddLine(From, To, e.is_visible ? colors["cESP_LineBots"] : colors["cESP_LineBotsHidden"], sync_float["fESP_LineThickness"]);
        } else {
            if (sync_bool["bESP_Line"]) draw->AddLine(From, To, e.is_visible ? colors["cESP_Line"] : colors["cESP_LineHidden"], sync_float["fESP_LineThickness"]);
        }
        if ((sync_bool["bESP_TeamID"] || sync_bool["bESP_Name"] || sync_bool["bESP_Distance"] || sync_bool["bESP_Health"]) && !e.bones.empty()) {
            ImVec2 head   = ToImVec2(e.bones.at("head"));
            ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_calf"));
            ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_calf"));
            ImVec2 feet   = (foot_l.y > foot_r.y) ? foot_l : foot_r;
            ImVec2 l    = ToImVec2(e.bones.at("limbs_l_calf"));
            ImVec2 r    = ToImVec2(e.bones.at("limbs_r_calf"));
            ImVec2 foot = ImVec2((l.x + r.x) / 2, (l.y + r.y) / 2);
            float height = fabsf(foot.y - head.y) * 2.0f;
            float width  = height / 2.0f;
            ImVec2 topLeft(head.x - width / 2, head.y - height * 0.2f);
            ImVec2 bottomRight(head.x + width / 2, topLeft.y + height);
            float boxHeight = feet.y - head.y;
            if (boxHeight <= 0) continue;
            float barWidth   = 7.0f;
            float barSpacing = 8.0f;
            if (sync_bool["bESP_Distance"]) {
                float distance = 1000.0f / boxHeight;
                if (distance > 300) distance = 300;
                std::string distanceText = std::to_string((int)distance) + "m";
                ImVec2 textSize = ImGui::CalcTextSize(distanceText.c_str());
                draw->AddText(ImVec2(feet.x - textSize.x / 2, feet.y + 5.0f), IM_COL32(0, 255, 0, 255), distanceText.c_str());
            }
            if (sync_bool["bESP_Health"]) {
                float healthPerc = (e.max_health > 0.0f) ? std::clamp(e.is_health / e.max_health, 0.0f, 1.0f) : 0.0f;
                float barH = bottomRight.y - topLeft.y;
                float barX = topLeft.x - barWidth - barSpacing;
                float barY = topLeft.y;
                ImU32 hpColor = e.is_knocked ? IM_COL32(255, 0, 0, 255) : (healthPerc > 0.5f ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255));
                draw->AddRectFilled({barX, barY}, {barX + barWidth, barY + barH}, IM_COL32(0, 0, 0, 180));
                draw->AddRectFilled({barX, barY + barH * (1.0f - healthPerc)}, {barX + barWidth, barY + barH}, hpColor);
            }
            if (sync_bool["bESP_Health"]) {
                float armorPerc = (e.max_armor > 0.0f) ? std::clamp(e.is_armor / e.max_armor, 0.0f, 1.0f) : 0.0f;
                float barH = bottomRight.y - topLeft.y;
                float barX = bottomRight.x + barSpacing;
                float barY = topLeft.y;
                ImU32 armorColor = armorPerc > 0.7f ? IM_COL32(255, 255, 0, 255) : (armorPerc > 0.3f ? IM_COL32(80, 160, 255, 255) : IM_COL32(255, 0, 0, 255));
                draw->AddRectFilled({barX, barY}, {barX + barWidth, barY + barH}, IM_COL32(0, 0, 0, 180));
                draw->AddRectFilled({barX, barY + barH * (1.0f - armorPerc)}, {barX + barWidth, barY + barH}, armorColor);
            }
            if (sync_bool["bESP_TeamID"] || sync_bool["bESP_Name"]) {
                std::string text;
                if (sync_bool["bESP_TeamID"]) text += std::to_string(e.team_id);
                if (sync_bool["bESP_TeamID"] && sync_bool["bESP_Name"]) text += " | ";
                if (sync_bool["bESP_Name"]) text += e.entity_name;
                ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
                draw->AddText(ImVec2(head.x - textSize.x / 2.0f, head.y - textSize.y - 4.0f), e.is_knocked ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 255, 255), text.c_str());
            }
        }
        if (sync_bool["bESP_Box"]) {
            if (!e.bones.count("limbs_l_calf") || !e.bones.count("limbs_r_calf")) continue;
            ImVec2 head = ToImVec2(e.bones.at("head"));
            ImVec2 l = ToImVec2(e.bones.at("limbs_l_calf"));
            ImVec2 r = ToImVec2(e.bones.at("limbs_r_calf"));
            ImVec2 foot = ImVec2((l.x + r.x) / 2, (l.y + r.y) / 2);
            float height = fabsf(foot.y - head.y) * 2.0f;
            float width  = height / 2.0f;
            ImVec2 topLeft(head.x - width / 2, head.y - height * 0.2f);
            ImVec2 bottomRight(head.x + width / 2, topLeft.y + height);
            ImDrawList* fg = ImGui::GetForegroundDrawList();
            ImU32 color = e.is_visible ? colors["cESP_Box"] : colors["cESP_BoxHidden"];
            float thickness = sync_float["fESP_LineThickness"];
            fg->AddRect(topLeft, bottomRight, color, 0.0f, 0, thickness);
        }
        if (sync_bool["bESP_DangerCircle"] && e.is_visible < 15.0f) {
            draw->AddCircle(To, 30.0f, IM_COL32(255, 0, 0, 180), 32, 2.0f);
            draw->AddText(ImVec2(To.x - 20, To.y + 25), IM_COL32(255, 0, 0, 255), "DANGER!");
        }
    }
    if (isPlayerBotEnabled) {
        float glWidth  = sync_float["fWidth"];
        int totalPlayers = players, totalBots = bots;
        ImDrawList* fg = ImGui::GetForegroundDrawList();
        if (totalPlayers + totalBots == 0) {
            fg->AddRectFilled({glWidth/2 - 67.5f, 35}, {glWidth/2 + 60, 75}, ImColor(0,0,0,120), 20);
            std::string txt = "CLEAR";
            ImVec2 size = ImGui::CalcTextSize(txt.c_str());
            fg->AddText(NULL, 35, { glWidth/2 - size.x/2 - 6, 35 + (40-size.y)/2 }, ImColor(255,255,255), txt.c_str());
        } else {
            fg->AddRectFilled({glWidth/2 - 69, 35}, {glWidth/2 + 60, 75}, ImColor(0,0,0,120), 20);
            ImVec2 boxPlayerMin = ImVec2(glWidth/2 - 60, 40);
            ImVec2 boxPlayerMax = ImVec2(glWidth/2 - 8, 70);
            ImVec2 boxBotMin    = ImVec2(glWidth/2 + 0, 40);
            ImVec2 boxBotMax    = ImVec2(glWidth/2 + 50, 70);
            fg->AddRectFilled(boxPlayerMin, boxPlayerMax, ImColor(255,0,0,200), 20);
            fg->AddRectFilled(boxBotMin, boxBotMax, ImColor(0,255,0,200), 20);
            std::string P = std::to_string(totalPlayers);
            std::string B = std::to_string(totalBots);
            ImVec2 pSize = ImGui::CalcTextSize(P.c_str());
            ImVec2 bSize = ImGui::CalcTextSize(B.c_str());
            float pCenterX = (boxPlayerMin.x + boxPlayerMax.x) / 2;
            float bCenterX = (boxBotMin.x + boxBotMax.x) / 2;
            float pCenterY = (boxPlayerMin.y + boxPlayerMax.y) / 2 - 2;
            float bCenterY = (boxBotMin.y + boxBotMax.y) / 2 - 2;
            fg->AddText(NULL, 35, { pCenterX - pSize.x/2 - 3, pCenterY - pSize.y/2 }, ImColor(255,255,255), P.c_str());
            fg->AddText(NULL, 35, { bCenterX - bSize.x/2 - 3, bCenterY - bSize.y/2 }, ImColor(255,255,255), B.c_str());
        }
    }
    update_aim_target();
    if (sync_bool["bAIM"] && aim_target_valid && aim_target_visible) {
        draw->AddLine(ImVec2(Width * 0.5f, Height * 0.5f), aim_target_screen, IM_COL32(255, 0, 0, 255), 2.0f);
    }
    if (!sync_bool["bAIM_NoTargetHideFov"] || (players || bots)) {
        if (sync_bool["bAIM_DrawFov"]) {
            draw->AddCircle(ImVec2(Width / 2, Height / 2), sync_float["fAIM_Fov"], IM_COL32(255, 0, 0, 255), 0, sync_float["fESP_LineThickness"]);
        }
    }
}

float deltaTime = 0.0f;
auto lastTime = std::chrono::high_resolution_clock::now();

void DrawBottomLeft()
{
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(now - lastTime).count();
    lastTime = now;
    deltaTime = dt;
    int fps = (int)(1.0f / deltaTime);
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    // Responsive HUD sizing
    float w = 320.0f;
    float h = 90.0f;
    if (screenSize.x < 1280) {
        w *= 0.8f;
        h *= 0.8f;
    }

    ImVec2 pos(40.0f, screenSize.y - h - 40.0f);

    // Glassmorphism effect
    draw_list->AddRectFilled(ImVec2(pos.x+5, pos.y+5), ImVec2(pos.x+w+5, pos.y+h+5), IM_COL32(0,0,0,40), 12.0f);
    draw_list->AddRectFilled(pos, ImVec2(pos.x + w, pos.y + h), IM_COL32(20, 20, 25, 220), 12.0f);
    draw_list->AddRectFilled(pos, ImVec2(pos.x + 8.0f, pos.y + h), IM_COL32(110, 100, 240, 255), 12.0f, ImDrawCornerFlags_Left);

    float titleSize = (screenSize.x < 1280) ? 22.0f : 26.0f;
    float subSize = (screenSize.x < 1280) ? 18.0f : 22.0f;

    draw_list->AddText(font::lexend_general_bold, titleSize, ImVec2(pos.x + 25, pos.y + 15), IM_COL32(230, 230, 230, 255), "STARCOOL ENGINE");

    char fpsBuf[32]; sprintf(fpsBuf, "%d FPS | SYSTEM STABLE", fps);
    draw_list->AddText(font::lexend_regular, subSize, ImVec2(pos.x + 25, pos.y + h - 35), IM_COL32(150, 150, 180, 255), fpsBuf);

    // Status indicator
    draw_list->AddCircleFilled(ImVec2(pos.x + w - 25, pos.y + 25), 5.0f, IM_COL32(0, 255, 150, 255));
}

void DrawMenu(ImGuiIO &io) {
    sync_with_py();
    sync_float[xorstr_("fHeight")] = Height;
    sync_float[xorstr_("fWidth")] = Width;
    static bool isLogin = true;
    static std::string err;
    if (!isLogin) {
        float loginW = 650.0f, loginH = 500.0f;
        if (Width < 700) loginW = Width * 0.95f;
        if (Height < 550) loginH = Height * 0.95f;

        ImGui::SetNextWindowSize(ImVec2(loginW, loginH));
        ImGui::SetNextWindowPos(ImVec2(Width / 2, Height / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        if (ImGui::Begin(xorstr_("AUTHENTICATION"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
            ImDrawList* draw = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetWindowPos();
            ImVec2 s = ImGui::GetWindowSize();
            draw->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y), IM_COL32(15, 15, 20, 255), 15.0f);

            ImGui::SetCursorPosY(60);
            ImGui::PushFont(font::lexend_general_bold);
            float textW = ImGui::CalcTextSize("GATEWAY ACCESS").x;
            ImGui::SetCursorPosX((s.x - textW) / 2);
            ImGui::TextColored(ImVec4(0.5f, 0.4f, 1.0f, 1.0f), "GATEWAY ACCESS");
            ImGui::PopFont();

            ImGui::SetCursorPosY(140);
            ImGui::SetCursorPosX(s.x * 0.1f);
            ImGui::BeginChild("LoginArea", ImVec2(s.x * 0.8f, s.y - 180), false);
            {
                ImGui::SetCursorPosY(20);
                ImGui::Text(xorstr_(ICON_FA_SHIELD_HALVED " ENTER ACCESS KEY"));
                static char s[150] = {0};
                ImGui::InputText(xorstr_("##key"), s, sizeof(s));
                if (ImGui::Button(xorstr_("PASTE"), ImVec2(480, 60))) {
                    auto key = getClipboard();
                    strncpy(s, key.c_str(), sizeof(s));
                }
                if (ImGui::Button(xorstr_("ACTIVATE"), ImVec2(480, 80))) {
                    err = Login(s);
                    if (err == "OK" && (bValid && g_Auth == g_Token)) isLogin = true;
                }
                if (!err.empty() && err != "OK") ImGui::TextColored(ImVec4(1,0,0,1), "Error: %s", err.c_str());
            }
            ImGui::EndChild();
            ImGui::End();
        }
    } else {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

        float menuW = 1050.0f;
        float menuH = 750.0f;
        if (Width < 1100 || Height < 800) {
            menuW = (float)Width * 0.96f;
            menuH = (float)Height * 0.96f;
        }

        ImGui::SetNextWindowSizeConstraints(ImVec2(800, 600), ImVec2(1400, 1000));
        ImGui::SetNextWindowSize(ImVec2(menuW, menuH), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(Width / 2, Height / 2), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));

        if (ImGui::Begin(xorstr_("STARCOOL_PRO"), nullptr, window_flags)) {
            static int active_tab = 0;
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetWindowPos();
            ImVec2 s = ImGui::GetWindowSize();
            draw_list->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y), IM_COL32(12, 12, 12, 255), 12.0f);
            draw_list->AddRectFilled(p, ImVec2(p.x + 240, p.y + s.y), IM_COL32(18, 18, 18, 255), 12.0f, ImDrawCornerFlags_Left);
            draw_list->AddRectFilled(ImVec2(p.x + 240, p.y + 80), ImVec2(p.x + s.x, p.y + 82), IM_COL32(40, 40, 40, 255));
            ImGui::SetCursorPos(ImVec2(30, 40));
            ImGui::PushFont(font::lexend_general_bold);
            ImGui::TextColored(ImVec4(0.5f, 0.4f, 1.0f, 1.0f), "STARCOOL");
            ImGui::PopFont();
            const char* tabs[] = { ICON_FA_USER " DASH", ICON_FA_CROSSHAIRS " AIM", ICON_FA_EYE " ESP", ICON_FA_GEARS " UTIL" };
            ImGui::SetCursorPosY(160);
            for(int i=0; i<4; i++) {
                ImGui::SetCursorPosX(15);
                bool selected = (active_tab == i);
                if (selected) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.4f, 1.0f, 1.0f));
                    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 10, p.y + 160 + i*90 + 5), ImVec2(p.x + 15, p.y + 160 + i*90 + 75), IM_COL32(120, 100, 255, 255), 2.0f);
                } else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                if (ImGui::Button(tabs[i], ImVec2(210, 80))) active_tab = i;
                ImGui::PopStyleColor();
                ImGui::Spacing();
            }
            ImGui::SetCursorPos(ImVec2(270, 30));
            ImGui::PushFont(font::lexend_bold);
            const char* titles[] = { "DASHBOARD", "COMBAT ENGINE", "VISUAL MATRIX", "UTILITY HUB" };
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), titles[active_tab]);
            ImGui::PopFont();
            ImGui::SetCursorPos(ImVec2(260, 100));
            ImGui::BeginChild("ContentFrame", ImVec2(s.x - 280, s.y - 120), false, ImGuiWindowFlags_NoScrollbar);
            {
                if (active_tab == 0) {
                    ImGui::Columns(2, NULL, false);
                    ImGui::SetColumnWidth(0, (s.x - 280) * 0.5f);
                    ImGui::SetColumnWidth(1, (s.x - 280) * 0.5f);

                    ImGui::BeginGroupPanel("SUBSCRIPTION", ImVec2(-1, 0));
                    ImGui::Text("Status  :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 0.5f, 1), mod_status.c_str());
                    ImGui::Text("Expiry  :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 0.8f, 0, 1), EXP.c_str());
                    ImGui::Text("User    :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.5f, 0.4f, 1.0f, 1), "Authenticated");
                    ImGui::EndGroupPanel();

                    ImGui::NextColumn();

                    ImGui::BeginGroupPanel("ENVIRONMENT", ImVec2(-1, 0));
                    ImGui::Text("Game    : %s", GameVersion.c_str());
                    ImGui::Text("Arch    : arm64-v8a");
                    ImGui::Text("Engine  : Starcool V2 Pro");
                    ImGui::EndGroupPanel();
                    ImGui::Columns(1);
                } else if (active_tab == 1) {
                    ImGui::Columns(2, NULL, false);
                    ImGui::SetColumnWidth(0, (s.x - 280) * 0.5f);
                    ImGui::SetColumnWidth(1, (s.x - 280) * 0.5f);

                    ImGui::BeginGroupPanel("AIMBOT MODES", ImVec2(-1, 0));
                    Toggle("Enable Aimbot", &sync_bool["bAIM"]);
                    Toggle("Visible Only", &sync_bool["bAIM_CheckVisibility"]);
                    Toggle("Ignore Knocked", &sync_bool["bAIM_IgnoreKnocked"]);
                    Toggle("Ignore Bots", &sync_bool["bAIM_IgnoreBots"]);
                    ImGui::EndGroupPanel();

                    ImGui::BeginGroupPanel("AIMBOT VISUALS", ImVec2(-1, 0));
                    Toggle("Draw FOV Circle", &sync_bool["bAIM_DrawFov"]);
                    Toggle("Hide FOV (No Target)", &sync_bool["bAIM_NoTargetHideFov"]);
                    Toggle("Show Snap Line", &sync_bool["bAIM_SnapLine"]);
                    ImGui::EndGroupPanel();

                    ImGui::NextColumn();

                    ImGui::BeginGroupPanel("PARAMETERS", ImVec2(-1, 0));
                    ImGui::Combo("Trigger", &sync_int["iAIM_Trigger"], aimbotTrigger, IM_ARRAYSIZE(aimbotTrigger));
                    ImGui::Combo("Target Bone", &sync_int["iAIM_TargetBone"], targetBone, IM_ARRAYSIZE(targetBone));
                    ImGui::SliderFloat("FOV Range", &sync_float["fAIM_Fov"], 30.0f, 1500.0f, "%.1f px");
                    ImGui::SliderFloat("FOV Offset Y", &sync_float["fAIM_FovOffsetY"], -500.0f, 500.0f, "%.1f");
                    ImGui::SliderFloat("Smoothness", &sync_float["fAIM_SnapStrength"], 0.01f, 3.0f, "%.2f");
                    ImGui::EndGroupPanel();
                    ImGui::Columns(1);
                } else if (active_tab == 2) {
                    ImGui::Columns(2, NULL, false);
                    ImGui::SetColumnWidth(0, (s.x - 280) * 0.5f);
                    ImGui::SetColumnWidth(1, (s.x - 280) * 0.5f);

                    ImGui::BeginGroupPanel("PLAYER ESP", ImVec2(-1, 0));
                    Toggle("Master ESP", &sync_bool["bESP"]);
                    Toggle("Show Boxes", &sync_bool["bESP_Box"]);
                    Toggle("Show Lines", &sync_bool["bESP_Line"]);
                    Toggle("Line to Bots", &sync_bool["bESP_LineBots"]);
                    Toggle("Show Skeleton", &sync_bool["bESP_Skeleton"]);
                    ImGui::EndGroupPanel();

                    ImGui::BeginGroupPanel("VISUAL SETTINGS", ImVec2(-1, 0));
                    ImGui::Combo("Line Origin", &sync_int["iESP_Point"], espPoint, IM_ARRAYSIZE(espPoint));
                    ImGui::SliderFloat("Line Thickness", &sync_float["fESP_LineThickness"], 1.0f, 10.0f, "%.1f");
                    ImGui::EndGroupPanel();

                    ImGui::NextColumn();

                    ImGui::BeginGroupPanel("ESP DETAILS", ImVec2(-1, 0));
                    Toggle("Name Tags", &sync_bool["bESP_Name"]);
                    Toggle("Team ID", &sync_bool["bESP_TeamID"]);
                    Toggle("Health Status", &sync_bool["bESP_Health"]);
                    Toggle("Distance", &sync_bool["bESP_Distance"]);
                    Toggle("Threat Alert (Danger)", &sync_bool["bESP_DangerCircle"]);
                    ImGui::EndGroupPanel();

                    ImGui::BeginGroupPanel("CHAMS & XRAY", ImVec2(-1, 0));
//                    Toggle("X-Ray Vision", &sync_bool["bXray"]);
                    if (ImGui::Combo("Chams Mode", &chamsint, Chams, IM_ARRAYSIZE(Chams))) {
                        // Action if needed
                    }
                    Toggle("Rainbow Chams", &enableRainbow);
                    Toggle("Rainbow Wall", &enableRainbowWall);
                    ImGui::EndGroupPanel();
                    ImGui::Columns(1);
                }
else if (active_tab == 3) {
                    ImGui::Columns(2, NULL, false);
                    ImGui::SetColumnWidth(0, (s.x - 280) * 0.5f);
                    ImGui::SetColumnWidth(1, (s.x - 280) * 0.5f);

                    ImGui::BeginGroupPanel("WEAPON MODS", ImVec2(-1, 0));
                    Toggle("No Recoil", &sync_bool["bNoRecoil"]);
                    Toggle("Less Recoil", &sync_bool["bLessRecoil"]);
                    Toggle("No Spread", &sync_bool["bNoSpread"]);
                    ImGui::EndGroupPanel();

                    ImGui::BeginGroupPanel("BULLET TRACK", ImVec2(-1, 0));
                    Toggle("Enable Track", &sync_bool["bBulletTrack"]);
                    ImGui::Combo("Track Bone", &sync_int["iBulletTrack_TargetBone"], targetBone, IM_ARRAYSIZE(targetBone));
                    ImGui::SliderFloat("Hit Chance", &sync_float["fBulletTrack_Probability"], 0.0f, 100.0f, "%.1f%%");
                    ImGui::EndGroupPanel();

                    ImGui::NextColumn();

                    ImGui::BeginGroupPanel("PHYSICS & OTHERS", ImVec2(-1, 0));
                    Toggle("Speed Hack", &sync_bool["bSpeed"]);
                    if(sync_bool["bSpeed"]) ImGui::SliderFloat("Value", &sync_float["fSpeed"], 1.0f, 15.0f, "x%.1f");
                    Toggle("Slide Hack", &sync_bool["bSlide"]);
                    Toggle("Wallhack (Environment)", &sync_bool["bWall"]);
                                        Toggle("Skin Hack", &sync_bool["bSkinHack"]);
                    if (sync_bool["bSkinHack"]) {
                        if (ImGui::Combo("Skin Select", &sync_int["iSkinIndex"], skinModes, IM_ARRAYSIZE(skinModes))) {
                            if (sync_int["iSkinIndex"] < IM_ARRAYSIZE(skinIDs) && skinIDs[sync_int["iSkinIndex"]] != -1) {
                                sync_int["iSelectedSkinID"] = skinIDs[sync_int["iSkinIndex"]];
                            }
                        }
                        // Jika pilih "USE MANUAL ID" (ID terakhir di skinIDs adalah -1)
                        if (sync_int["iSkinIndex"] < IM_ARRAYSIZE(skinIDs) && skinIDs[sync_int["iSkinIndex"]] == -1) {
                            if (ImGui::InputInt("Manual ID", &sync_int["iCustomSkinID"])) {
                                sync_int["iSelectedSkinID"] = sync_int["iCustomSkinID"];
                            }
                        }
                    }
                    ImGui::EndGroupPanel();
                    ImGui::Columns(1);
                }
            }
            ImGui::EndChild();
            ImGui::End();
        }
    }
}

static bool setup = false;
DEFINES(EGLBoolean, Draw, EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &Width);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &Height);
    if (Width <= 0 || Height <= 0) return _Draw(dpy, surface);
    if (!setup) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = NULL;
        static const ImWchar icons_ranges[] = {0xf000, 0xf3ff, 0};
        ImFontConfig font_config, icons_config, cfg;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 2.5;
        icons_config.OversampleV = 2.5;
        io.Fonts->AddFontFromMemoryTTF((void *)SabFont, (int)SabFont_size, 24.f, &font_config, io.Fonts->GetGlyphRangesDefault());
        io.Fonts->AddFontFromMemoryTTF(fa_solid_900_ttf, fa_solid_900_ttf_len, 20.0f, &icons_config, icons_ranges);
        flamee = io.Fonts->AddFontFromMemoryTTF((void *)Yaser_data, Mod_size, 22.0f, &icons_config, io.Fonts->GetGlyphRangesCyrillic());
        Japan = io.Fonts->AddFontFromMemoryTTF((void *)Darmen_data, Darmen_size, 22.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
        ubu_0 = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(ubuntu_0), ubuntu_0_size, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
        pubg_font = io.Fonts->AddFontFromMemoryTTF((void *)default_f, sizeof default_f, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
        font::lexend_general_bold = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(lexend_bold), lexend_bold_size, 22.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
        font::lexend_bold = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(lexend_regular), lexend_regular_size, 22.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
        font::lexend_regular = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(lexend_regular), lexend_regular_size, 22.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
        font::icomoon = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(icomoon), icomoon_size, 24.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
        font::icomoon_tabs = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(icomoon), icomoon_size, 28.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
        font::icomoon_widget = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(icomoon_widget), icomoon_widget_size, 28.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
        font::icomoon_widget2 = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(icomoon), icomoon_size, 28.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
        ImGui_ImplOpenGL3_Init("#version 300 es");
        ImGuiStyle &style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.25f, 0.50f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.50f, 0.40f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.40f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.50f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.12f, 0.12f, 0.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.50f, 0.40f, 1.00f, 0.20f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.40f, 1.00f, 0.40f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.40f, 1.00f, 0.60f);
        style.WindowRounding = 12.0f;
        style.FrameRounding = 6.0f;
        style.ChildRounding = 10.0f;
        style.GrabRounding = 6.0f;
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 10.0f;
        style.WindowPadding = ImVec2(20, 20);
        style.ItemSpacing = ImVec2(12, 18);
        style.ScaleAllSizes(2.5f);
        setup = true;
    }
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)Width, (float)Height);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(Width, Height);
    NewFrame();
    DrawBottomLeft();
    UpdateRainbow();
    ClearImGuiWindowTracking();
    noMore(GetBackgroundDrawList());
    DrawMenu(io);
    EndFrame();
    Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return _Draw(dpy, surface);
}
