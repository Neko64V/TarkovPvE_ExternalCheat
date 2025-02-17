#pragma once
#include "../Framework/ImGui/imgui.h"
#include "../Framework/ImGui/imgui_impl_win32.h"
#include "../Framework/ImGui/imgui_impl_dx11.h"
#include "../Framework/ImGui/Custom.h"
#include "SDK/CPlayer/CPlayer.h"
#include "SDK/CExfil/CExfil.h"
#include "SDK/CItem/CItem.h"

class CFramework
{
public:
    void UpdateList();
    void UpdateStaticList();
	void RenderInfo();
	void RenderMenu();
	void RenderESP();
    void MiscAll();
    ~CFramework();
private:
    EFT* tarkov = new EFT();
    CPlayer local, *pLocal = &local;
    std::vector<CPlayer> EntityList;
    std::vector<CExfil> ExfilList;
    std::vector<CItem> ItemList;
    std::vector<uintptr_t> GrenadeList;

    // Colors
    ImColor Col_ESP_PMC = { 1.f, 0.f, 0.85f, 1.f };
    ImColor Col_ESP_Team = { 0.f, 1.f, 0.f, 0.f };
    ImColor Col_ESP_Scav = { 1.f, 1.f, 1.f, 1.f };
    ImColor Col_ESP_pScav = { 1.f, 1.f, 0.f, 1.f };
    ImColor Col_ESP_SpecialScav = { 1.f, 0.5f, 0.f, 1.f };
    ImColor Col_ESP_Boss = { 1.f, 0.f, 0.f, 1.f };
    ImColor Col_ESP_Filled = { 0.f, 0.f, 0.f, 0.25f };

    ImColor Col_ESP_ExfilOpen = { 0.f, 1.f, 0.f, 0.5f };
    ImColor Col_ESP_ExfilClose = { 1.f, 0.3f, 0.f, 0.5f };

    ImColor Col_ESP_RareItem = { 1.f, 0.f, 0.f, 0.9f };

    ImColor CrosshairColor = { 0.f, 1.f, 0.f, 1.f };

    void GetESPInfo(const int& SpawnType, std::string& vOutStr, ImColor& vOutColor);

    void DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width)
    {
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2((int)a.x, (int)a.y), ImVec2((int)b.x, (int)b.y), color, width);
    }
    void RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
    {
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
    }
    void HealthBar(float x, float y, float w, float h, int value, int v_max)
    {
        RectFilled(x, y, x + w, y + h, ImColor(0.f, 0.f, 0.f, 0.725f), 0.f, 0);
        RectFilled(x, y, x + w, y + ((h / float(v_max)) * (float)value), ImColor(min(510 * (v_max - value) / 100, 255), min(510 * value / 100, 255), 25, 255), 0.0f, 0);
    }
    void Circle(ImVec2 pos, float fov_size, ImColor color)
    {
        ImGui::GetBackgroundDrawList()->AddCircle(pos, fov_size, color, 100, 0);
    }
    void String(ImVec2 pos, ImColor color, const char* text)
    {
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, color, text, text + strlen(text), 1024, 0);
    }
    void StringEx(ImVec2 pos, ImColor color, float font_size, const char* text)
    {
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), font_size, ImVec2(pos.x + 1.f, pos.y + 1.f), ImColor(0.f, 0.f, 0.f, 1.f), text, text + strlen(text), 1024, 0);
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), font_size, pos, color, text, text + strlen(text), 1024, 0);
    }
};