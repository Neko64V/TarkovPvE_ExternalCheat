#pragma once
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../Utils/Config/Config.h"
#include "../SDK/CExfil/Exfil.h"
#include "../SDK/CItem/Item.h"
#include "../SDK/CPlayer/Player.h"
#include <vector>

class Cheat
{
public:
    void Init();
    void Misc();
    void UpdateList();
    void RenderInfo();
    void RenderMenu();
    void RenderESP();
private:

    CPlayer local{};
    int ItemCount = 0;
    ImColor SetColor(int SpawnType);

    float recoilVal = 0.f;

    // List
    std::vector<CPlayer> EntityList;
    std::vector<CItem> ItemList;
    std::vector<CExfil> ExfilList;

    // Colors
    ImColor Col_ESP_ExfilOpen = { 0.f, 1.f, 0.f, 0.5f };
    ImColor Col_ESP_ExfilClose = { 1.f, 0.3f, 0.f, 0.5f };

    ImColor Col_ESP_PMC = { 1.f, 0.f, 0.85f, 1.f };
    ImColor Col_ESP_Team = { 0.f, 1.f, 0.f, 0.f };
    ImColor Col_ESP_Scav = { 1.f, 1.f, 1.f, 1.f };
    ImColor Col_ESP_pScav = { 1.f, 1.f, 0.f, 1.f };
    ImColor Col_ESP_SpecialScav = { 1.f, 0.5f, 0.f, 1.f };
    ImColor Col_ESP_Boss = { 1.f, 0.f, 0.f, 1.f };
    ImColor Col_ESP_Filled = { 0.f, 0.f, 0.f, 0.25f };

    void DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width)
    {
        ImGui::GetBackgroundDrawList()->AddLine(a, b, color, width);
    }
    void Text(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
    {
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, color, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
    }
    void String(ImVec2 pos, ImColor color, const char* text)
    {
        Text(pos, color, text, text + strlen(text), 128, 0);
    }
    void RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
    {
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
    }
    void HealthBar(float x, float y, float w, float h, int value, int v_max)
    {
        RectFilled(x, y, x + w, y + h, ImColor(0.f, 0.f, 0.f, 0.75f), 0.f, 0);
        RectFilled(x, y, x + w, y + ((h / float(v_max)) * (float)value), ImColor(min(510 * (v_max - value) / 100, 255), min(510 * value / 100, 255), 25, 255), 0.0f, 0);
    }
};