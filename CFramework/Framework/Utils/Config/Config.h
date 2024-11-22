#pragma once
#include <Windows.h>

struct Globals
{
    // System
    bool Run = true;
    bool ShowMenu = false;

    // GameWindow Data
    HWND GameHwnd;
    RECT GameRect{};
    POINT GamePoint{};

    // Visual
    bool g_ESP      = true;
    bool g_ExfilESP      = true;
    bool g_ESP_Line      = false;
    bool g_ESP_Box       = true;
    bool g_ESP_BoxFilled = false;
    bool g_ESP_Skeleton  = true;
    bool g_ESP_Distance  = true;
    bool g_ESP_Name      = true;
    bool g_ESP_HealthBar = true;
    int  g_ESP_BoxStyle  = 1;
    float g_ESP_MaxDistance = 1500.f;
   
    // Misc


    // System
    bool g_StreamProof = false;
    bool g_Crosshair = false;
    int g_CrosshairSize = 3;
    int g_CrosshairType = 0;

    // Key
    int g_MenuKey = VK_INSERT;
    int g_AimKey0 = VK_RBUTTON;
    int g_AimKey1 = VK_LBUTTON;
};

extern Globals g;