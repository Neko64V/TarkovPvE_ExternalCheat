#include "Cheat.h"
#include "..\ImGui\Font\RobotoRegular.h"
#include "../ImGui/Custom.h"
#pragma comment(lib, "freetype.lib")

std::vector<std::string> MenuSelectList = { "AimBot", "Visual", "Misc" };
const char* bStyleList[] = { "Simple", "Cornered" };

void Cheat::Init()
{
    // LoadFonts
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromMemoryCompressedTTF(RobotoRegular_compressed_data, RobotoRegular_compressed_size, 14.f, nullptr);
    io.Fonts->Build();
}

void Cheat::RenderInfo()
{
    // FrameRate
    std::string FrameText = "Tarkov External | " + std::to_string((int)ImGui::GetIO().Framerate) + " FPS";
    ImGui::GetBackgroundDrawList()->AddText(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), FrameText.c_str());

    if (g.NoSway && !local.IsAiming())
    {
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(g.GameSize.right / 2.f - 5.f, g.GameSize.bottom / 2.f), ImVec2(g.GameSize.right / 2.f + 6.f, g.GameSize.bottom / 2.f), ImColor(1.f, 1.f, 1.f, 1.f), 1.f);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(g.GameSize.right / 2.f, g.GameSize.bottom / 2.f - 5.f), ImVec2(g.GameSize.right / 2.f, g.GameSize.bottom / 2.f + 6.f), ImColor(1.f, 1.f, 1.f, 1.f), 1.f);
    }

    // FOV
    if (g.AimBot && g.DrawFov)
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(g.GameSize.right / 2.f, g.GameSize.bottom / 2.f), g.Aim_Fov, ImColor(1.f, 1.f, 1.f, 1.f));

    ImGui::SetNextWindowPos(ImVec2(50.f, 100.f));
    ImGui::Begin("EFT DEBUG WINDOW", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

    ImGui::Text("[+] Entity");
    ImGui::Text("Item : %d", ItemCount);
    ImGui::NewLine();
    ImGui::Text("[+] Local");
    ImGui::Text("HP : %.f / %.f", local.m_pHealth, local.m_pHealthMax);
    ImGui::Text("Pos : %.f, %.f, %.f", local.BasePos.x, local.BasePos.y, local.BasePos.z);

    ImGui::End();
}

void Cheat::RenderMenu()
{
    // Setup
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImGuiStyle& style = ImGui::GetStyle();
    static float DefaultSpacing = style.ItemSpacing.y;
    auto FramePadding = style.FramePadding;
    static int Index = 0;

    ImGui::SetNextWindowBgAlpha(0.975f);
    ImGui::SetNextWindowSize(ImVec2(725.f, 450.f));
    ImGui::Begin(xorstr_("Tarkov PvE [ EXTERNAL ]"), &g.ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    //---// Clild 0 //-----------------------------------//
    ImGui::BeginChild(xorstr_("##SelectChild"), ImVec2(150.f, ImGui::GetContentRegionAvail().y), false);

    ImGui::NewLine();

    for (int i = 0; i < MenuSelectList.size(); i++)
    {
        if (ImGui::CustomButton(MenuSelectList[i].c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 35.f), Index == i ? true : false))
            Index = i;
    }

    ImGui::EndChild();
    //---// Clild 0 //-----------------------------------//

    ImGui::SameLine();

    //---// Clild 1 //-----------------------------------//
    ImGui::BeginChild("BaseChild##2", ImVec2((ImGui::GetContentRegionAvail().x / 2.f) - 12.f, ImGui::GetContentRegionAvail().y));

    ImGui::Spacing();

    switch (Index)
    {
    case 0:
        // ToDo
        break;
    case 1:
        ImGui::Text("Visual");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Player ESP", &g.PlayerESP);
        ImGui::Checkbox("Item ESP", &g.ItemESP);
        ImGui::Checkbox("Exfil ESP", &g.ExfilESP);

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("ESP Options");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Box", &g.ESP_Box);
        ImGui::Checkbox("BoxFilled", &g.ESP_BoxFilled);
        ImGui::Checkbox("Line", &g.ESP_Line);
        ImGui::Checkbox("Name", &g.ESP_Name);
        ImGui::Checkbox("Distance", &g.ESP_Distance);
        ImGui::Checkbox("Healthbar", &g.ESP_HealthBar);
        ImGui::Checkbox("Skeleton", &g.ESP_Skeleton);
        break;
    case 2:
        ImGui::Text("System");
        ImGui::Separator();  ImGui::Spacing();
        ImGui::Checkbox("StreamProof", &g.StreamProof);

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("Game");
        ImGui::Separator();  ImGui::Spacing();
        ImGui::Checkbox("InfStamina", &g.InfStamina);
        ImGui::Checkbox("NoFallDamage", &g.NoFallDamage);
        ImGui::Checkbox("NoSway", &g.NoSway);
        break;
    case 3:
        if (ImGui::Button("Exit", ImVec2(ImGui::GetContentRegionAvail().x, 30.f)))
            g.Run = false;
        break;
    default:
        break;
    }

    ImGui::EndChild();
    //---------------------------------------------------//

    ImGui::SameLine();

    //---// Right //--------------------------------------//
    ImGui::BeginChild(xorstr_("##RightChild"), ImVec2(ImGui::GetContentRegionAvail()), false);

    ImGui::Spacing();

    switch (Index)
    {
    case 0:
        // ToDo
        break;
    case 1:
        ImGui::Text("ESP Distance");
        ImGui::Separator();
        ImGui::Spacing();

        style.ItemSpacing.y = 3.f;
        ImGui::CustomSliderFloat("Player", "##pDist", &g.ESP_MaxDistance, 100.f, 1500.f);
        ImGui::CustomSliderFloat("Item", "##itmDist", &g.Item_MaxDistance, 25.f, 100.f);
        style.ItemSpacing.y = DefaultSpacing;

        ImGui::Spacing();

        ImGui::Combo("BoxStyle", &g.ESP_BoxStyle, bStyleList, IM_ARRAYSIZE(bStyleList));

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("ESP Colors");
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::TreeNode("Player"))
        {
            ImGui::ColorEdit4("PMC", &Col_ESP_PMC.Value.x);
            ImGui::ColorEdit4("Team", &Col_ESP_Team.Value.x);

            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Scav"))
        {
            ImGui::ColorEdit4("Scav", &Col_ESP_Scav.Value.x);
            ImGui::ColorEdit4("Player", &Col_ESP_pScav.Value.x);
            ImGui::ColorEdit4("Supecial", &Col_ESP_SpecialScav.Value.x);
            ImGui::ColorEdit4("Boss", &Col_ESP_Boss.Value.x);

            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Game"))
        {
            ImGui::BulletText("Exfil");
            ImGui::ColorEdit4("Exfil (Open)", &Col_ESP_ExfilOpen.Value.x);
            ImGui::ColorEdit4("Exfil (Close)", &Col_ESP_ExfilClose.Value.x);

            ImGui::NewLine();
            ImGui::BulletText("Item");

            ImGui::TreePop();
        }
        break;
    case 2:

        break;
    case 3:

        break;
    default:
        break;
    }

    ImGui::EndChild();
    ImGui::End();
}

void Cheat::RenderESP()
{
    // GetLocal
    CPlayer* pLocal = &local;
    if (!pLocal->Update())
        return;

    // Player
    for (auto& player : EntityList)
    {
        CPlayer* pEntity = &player;
        if (!pEntity->Update())
            continue;

        float Distance = GetDistance(pEntity->BasePos, pLocal->BasePos);

        if (g.ESP_MaxDistance < Distance)
            continue;

        Vector2 g_Screen{}, g_hScreen{}, g_NeckScreen{};
        if (!EFT.WorldToScreen(pEntity->BasePos, g_Screen) || !EFT.WorldToScreen(pEntity->HeadPos, g_hScreen) || !EFT.WorldToScreen(pEntity->NeckPos, g_NeckScreen))
            continue;

        float NeckToHead = g_NeckScreen.y - g_hScreen.y;
        float pHeight = g_Screen.y - g_hScreen.y + (NeckToHead * 3.f);
        float pWidth = pHeight / 3.5f;
        ImColor pColor = SetColor(pEntity->m_pSpawnType);

        if (pEntity->m_pSpawnType == Vehicle_BTR) {
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(g_Screen.x, g_Screen.y - 2.f), 2.f, ImColor(1.f, 1.f, 1.f, 1.f), 0.f);
            String(ImVec2(g_Screen.x - (ImGui::CalcTextSize("BTR").x / 2.f), g_Screen.y), ImColor(1.f, 1.f, 1.f, 1.f), "BTR");
            continue;
        }

        // Line
        if (g.ESP_Line)
            DrawLine(ImVec2(g.GameSize.right / 2.f, g.GameSize.bottom), ImVec2(g_Screen.x, g_Screen.y), pColor, 1.f);

        // 2D Box
        if (g.ESP_Box)
        {
            float bScale = 1.5f;

            if (g.ESP_BoxFilled)
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(g_Screen.x - pWidth, g_Screen.y - pHeight), ImVec2(g_Screen.x + pWidth, g_Screen.y), Col_ESP_Filled, 1.f);

            switch (g.ESP_BoxStyle)
            {
            case 0:
                DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y - pHeight), ImVec2(g_Screen.x + pWidth, g_Screen.y - pHeight), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y), ImVec2(g_Screen.x - pWidth, g_Screen.y - pHeight), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + pWidth, g_Screen.y), ImVec2(g_Screen.x + pWidth, g_Screen.y - pHeight), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y), ImVec2(g_Screen.x + pWidth, g_Screen.y), pColor, 1.f);
                break;
            case 1:
                DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y - pHeight), ImVec2(g_Screen.x - pWidth + (pWidth / bScale), g_Screen.y - pHeight), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + pWidth, g_Screen.y - pHeight), ImVec2(g_Screen.x + pWidth - (pWidth / bScale), g_Screen.y - pHeight), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y - pHeight), ImVec2(g_Screen.x - pWidth, g_Screen.y - pHeight + (pWidth / bScale)), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + pWidth, g_Screen.y - pHeight), ImVec2(g_Screen.x + pWidth, g_Screen.y - pHeight + (pWidth / bScale)), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y), ImVec2(g_Screen.x - pWidth, g_Screen.y - (pWidth / bScale)), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + pWidth, g_Screen.y), ImVec2(g_Screen.x + pWidth, g_Screen.y - (pWidth / bScale)), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y), ImVec2(g_Screen.x - pWidth + (pWidth / bScale), g_Screen.y), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + pWidth, g_Screen.y), ImVec2(g_Screen.x + pWidth - (pWidth / bScale), g_Screen.y), pColor, 1.f);
                break;
            default:
                break;
            }
        }

        // Distance
        if (g.ESP_Distance)
        {
            std::string DistanceStr = std::to_string((int)Distance) + "m";
            String(ImVec2(g_Screen.x - (ImGui::CalcTextSize(DistanceStr.c_str()).x / 2.f), g_Screen.y), ImColor(1.f, 1.f, 1.f, 1.f), DistanceStr.c_str());
        }

        // Name
        if (g.ESP_Name && pEntity->m_pSpawnType != Scav && pEntity->m_pSpawnType != Scav_Sniper)
        {
            std::string name_str = pEntity->Name;

            if (!name_str.compare("unknown"))
                name_str += "[" + std::to_string(pEntity->m_pSpawnType) + "]";
            String(ImVec2(g_Screen.x - (ImGui::CalcTextSize(name_str.c_str()).x / 2.f), g_Screen.y - pHeight - 15.f), ImColor(1.f, 1.f, 1.f, 1.f), name_str.c_str());
        }

        // Healthbar
        if (g.ESP_HealthBar)
            HealthBar(g_Screen.x - pWidth - 4.f, g_Screen.y, 2.f, -pHeight, pEntity->m_pHealth, pEntity->m_pHealthMax);

        // Skeleton
        if (g.ESP_Skeleton && Distance < 500.f)
        {
            int list[][2] = { { Neck, Pelvis }, { Neck, LeftForearm }, { LeftForearm, LeftPalm }, { Neck, RightForearm }, { RightForearm, RightPalm }, { Pelvis, LeftThigh }, { LeftThigh, LeftFoot }, { Pelvis, RightThigh }, { RightThigh, RightFoot } };

            for (int j = 0; j < 9; j++)
            {
                Vector3 bone1 = pEntity->GetBonePositionByID(list[j][0]);
                Vector3 bone2 = pEntity->GetBonePositionByID(list[j][1]);

                Vector2 bOut1{}, bOut2{};
                if (Vec3_Empty(bone1) || Vec3_Empty(bone2))
                    break;
                else if (!EFT.WorldToScreen(bone1, bOut1) || !EFT.WorldToScreen(bone2, bOut2))
                    break;

                DrawLine(ImVec2(bOut1.x, bOut1.y), ImVec2(bOut2.x, bOut2.y), pColor, 1.f);
            }

            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(g_hScreen.x, g_hScreen.y - (NeckToHead / 3.f)), (NeckToHead * 1.5f), pColor, 0.f);
        }
    }

    // Item
    if (g.ItemESP)
    {
        for (auto& item : ItemList)
        {
            CItem* pItem = &item;

            float itemDistance = GetDistance(pLocal->BasePos, pItem->ItemPos);

            if (g.Item_MaxDistance > itemDistance)
            {
                Vector2 g_ItemScreen{};
                if (!EFT.WorldToScreen(pItem->ItemPos, g_ItemScreen))
                    continue;

                std::string ItemString = pItem->ItemName;

                // ��
                ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(g_ItemScreen.x, g_ItemScreen.y - 2.f), 2.f, ImColor(1.f, 1.f, 1.f, 1.f), 0.f);
                String(ImVec2(g_ItemScreen.x - (ImGui::CalcTextSize(ItemString.c_str()).x / 2.f), g_ItemScreen.y), ImColor(1.f, 1.f, 1.f, 1.f), ItemString.c_str());
            }
        }
    }

    // Exfil
    if (g.ExfilESP)
    {
        for (auto& exfil : ExfilList)
        {
            CExfil* pExfil = &exfil;
            Vector2 g_ExfilScreen{};
            if (!EFT.WorldToScreen(pExfil->m_pVecLocation, g_ExfilScreen))
                continue;

            ImColor ExfilColor = pExfil->m_pStatus == NOTREADY ? Col_ESP_ExfilClose : Col_ESP_ExfilOpen;

            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(g_ExfilScreen.x, g_ExfilScreen.y - 2.f), 2.f, ExfilColor, 0.f);
            String(ImVec2(g_ExfilScreen.x - (ImGui::CalcTextSize(pExfil->ExfilName.c_str()).x / 2.f), g_ExfilScreen.y), ExfilColor, pExfil->ExfilName.c_str());
        }
    }
}