#pragma once
#include <string>
#include <Windows.h>
#include "../xorstr.hpp"

struct Globals
{
    // System
    bool Run = false;
    bool ShowMenu = false;

    // GameData
    HWND GameHwnd;
    RECT GameSize{};
    POINT GamePoint{};
    std::string ProcName;

    // AimBot
    bool AimBot   = true;
    bool Aim_Prediction = true;
    bool Aim_NPC  = false;
    bool DrawFov  = true;
    bool RainbowFov = false;
    float Aim_Fov = 150.f;
    int Aim_Bone = 0;
    int Aim_Type = 0;
    float Aim_Smooth = 1.f;
    float Aim_MaxDistance = 225.f;

    // Visual
    bool PlayerESP  = true;
    bool ExfilESP = true;
    bool ItemESP = false;

    float Item_MaxDistance = 100.f;

    bool NPC_ESP    = false;
    bool ESP_Box    = true;
    bool ESP_BoxFilled = false;
    bool ESP_Line      = false;
    bool ESP_Name      = true;
    bool ESP_Skeleton  = true;
    bool ESP_Distance  = true;
    bool ESP_HealthBar = true;
    float ESP_MaxDistance = 300.f;
    int ESP_BoxStyle = 1;
    int ESP_SkeletonColor = 0;

    // Misc
    bool InfStamina = true;
    bool NoFallDamage = true;
    bool NoSway = false;

    bool StreamProof = false;

    bool Crosshair = false;
    int CrosshairSize = 3;
    int CrosshairType = 0;

    // Key
    int MenuKey = VK_INSERT;
    int AimKey0 = VK_RBUTTON;
    int AimKey1 = VK_LBUTTON;

    // KeyBinder
    int BindingID = 0;
};

extern Globals g;
extern bool IsKeyDown(int VK);
extern const char* KeyNames[];