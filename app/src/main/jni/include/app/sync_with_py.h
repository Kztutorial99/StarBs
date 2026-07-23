#pragma once

#include <iostream>
#include <map>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "utils/xorstr.hpp"
#include "utils/json.hpp"
using json = nlohmann::json;

#include "includes.h"


#define FIFO_PATH xorstr_("/data/data/" PACKAGE_NAME "/imgui/sync")

static std::map<std::string, bool> sync_bool = {
    {xorstr_("bAIM"), false},
    {xorstr_("bAIM_IgnoreBots"), false},
    {xorstr_("bAIM_CheckVisibility"), false},
    {xorstr_("bAIM_IgnoreKnocked"), false},
    {xorstr_("bAIM_SnapLine"), false},
    {xorstr_("bAIM_DrawFov"), false},
    {xorstr_("bAIM_NoTargetHideFov"), false},

    {xorstr_("bESP"), false},
    {xorstr_("bESP_Line"), false},
    {xorstr_("bESP_LineBots"), false},
    {xorstr_("bESP_Skeleton"), false},
    {xorstr_("bESP_Box"), false},
    {xorstr_("bESP_Health"), false},
    {xorstr_("bESP_Name"), false},
    {xorstr_("bESP_TeamID"), false},
    {xorstr_("bESP_Distance"), false},
    {xorstr_("bESP_DangerCircle"), false},
    {xorstr_("bWall"), true},

    {xorstr_("bBulletTrack"), false},
    // {xorstr_("bBulletTrack_IgnoreKnocked"), false},
    // {xorstr_("bBulletTrack_IgnoreBots"), false},

    {xorstr_("bXray"), false},
    {xorstr_("bSkinHack"), false},
    {xorstr_("bSpeed"), false},
    {xorstr_("bSlide"), false},
    {xorstr_("bNoRecoil"), false},
    {xorstr_("bNoSpread"), false},
    {xorstr_("bLessRecoil"), true},


};

static std::map<std::string, float> sync_float = {
    {xorstr_("fAIM_Fov"), 100.0f},
    {xorstr_("fSpeed"), 1.0f},
	{xorstr_("fAIM_FovOffsetY"), 1.0f},
    {xorstr_("fESP_LineThickness"), 2.0f},
    {xorstr_("fAIM_SnapStrength"), 0.1f},
    {xorstr_("fBulletTrack_Probability"), 70.0f},
    {xorstr_("fHeight"), 0.0f},
    {xorstr_("fWidth"), 0.0f}
};

static std::map<std::string, int> sync_int = {
    {xorstr_("iAIM_TargetBone"), 1},
    {xorstr_("iAIM_Trigger"), 0},
    {xorstr_("iESP_Point"), 2},
    {xorstr_("iBulletTrack_TargetBone"), 0},
    {xorstr_("iSkinIndex"), 0},
    {xorstr_("iCustomSkinID"), 0},
    {xorstr_("iSelectedSkinID"), 0},
};

static const char* skinModes[] = {
    "Random/Default",
    "[M4A1] Glacier", "[M4A1] Eternal",
    "[SCAR] Skin 1", "[SCAR] Skin 2",
    "[AK-47] Gold/Legend",
    "[VSS] Special",
    "[KAG] Elite",
    "[AUG] Skin 1", "[AUG] Skin 2",
    "[AR97] Custom",
    "[MCX] Modern",
    "[FAL] Classic", "[FAL] Skin 1", "[FAL] Skin 2",
    "[GALIL] Skin 1", "[GALIL] Skin 2",
    "[FN2000] Tech",
    "[RPK] Heavy",
    "[QBZ] Azure",
    "[PKM] Beast",
    "[M700] Sniper 1", "[M700] Sniper 2", "[M700] Sniper 3",
    "[KAR98] Classic", "[KAR98] Modern",
    "[KALA] Skin 1", "[KALA] Skin 2", "[KALA] Skin 3", "[KALA] Skin 4",
    "[SVD] Precision",
    "[BOW] Dragon",
    "[ORIGIN12] Skin 1", "[ORIGIN12] Skin 2",
    "[MP155] Shotgun",
    "[GLOCK] Pistol",
    "[DEAGLE] Chrome",
    "[MP5] Skin 1", "[MP5] Skin 2",
    "[VECTOR] Neon",
    "[URB] Urban",
    "[INP9] Compact",
    "[P90] Speed",
    "[UZI] Skin 1", "[UZI] Skin 2",
    "[PDG] Secret",
    "[PISO] Default",
    "[KAPAK] Melee",
    "USE MANUAL ID"
};

static int skinIDs[] = {
    0,
    11199011, 11199015,
    231199007, 231199009,
    1011990125,
    251100016,
    1411990110,
    291199017, 2911990155,
    2711990135,
    311199006,
    11100011, 321199007, 321199014,
    381199006, 381199003,
    371199005,
    411199001,
    391199009,
    341199010,
    181100008, 181100019, 181100018,
    331100003, 331199011,
    131199005, 131100010, 131199009, 131100014,
    401199001,
    221100008,
    61100011, 61100013,
    201100011,
    41100015,
    151100011,
    21100002, 21100009,
    81199007,
    161199008,
    171199006,
    241100010,
    351199006, 351199005,
    31000010,
    51000001,
    21000010,
    -1 // Flag for manual ID
};


static const char* targetBone[] = { "Head", "Chest", "L Hand", "R Hand", "L Foot", "R Foot", "Pelvis" };
static const char* aimbotTrigger[] = { "Aim+Shoot", "Aim", "Shoot", "Always" };
static const char* espPoint[] = { "Bottom", "Center", "Top" };

static void sync_with_py() {
    json j = json::object();
    
    for (const auto& pair : sync_bool)  j[pair.first] = pair.second;
    for (const auto& pair : sync_float) j[pair.first] = pair.second;
    for (const auto& pair : sync_int)   j[pair.first] = pair.second;
    
    std::string serialized = j.dump();
    
    int fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
    if (fd >= 0) {
        write(fd, serialized.c_str(), serialized.length());
        write(fd, "\n", 1);
        close(fd);
    }
}
