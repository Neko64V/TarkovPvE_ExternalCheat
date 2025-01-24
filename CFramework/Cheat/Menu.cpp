#include "FrameCore.h"

// ImGui::Combo/ImGui::List等で使う文字列群
const char* BoxTypeList[] = { "Simple", "Cornered" };
const char* CrosshairList[] = { "Cross", "Circle" };
std::vector<std::string> MenuSelectList = { "Visual", "Misc", "System" };

void CFramework::RenderMenu()
{
    // Setup
    static int Index = 0;

    ImGui::SetNextWindowBgAlpha(0.975f);
    ImGui::SetNextWindowSize(ImVec2(725.f, 450.f));
    ImGui::Begin("Tarkov PvE [ EXTERNAL ]", &g.ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    //---// Clild 0 //-----------------------------------//
    ImGui::BeginChild("##SelectChild", ImVec2(150.f, ImGui::GetContentRegionAvail().y), false);

    ImGui::NewLine();

    for (int i = 0; i < MenuSelectList.size(); i++) {
        if (ImGui::CustomButton(MenuSelectList[i].c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 35.f), Index == i ? true : false))
            Index = i;
    }

    ImGui::EndChild();
    //---// Clild 0 End //-------------------------------//

    ImGui::SameLine();

    //---// Clild 1 //-----------------------------------//
    ImGui::BeginChild("##ContextChild", ImVec2(ImGui::GetContentRegionAvail()), false);

    //---// Left //--------------------------------------//
    ImGui::BeginChild("##LeftChild", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 16.f, ImGui::GetContentRegionAvail().y), false);

    ImGui::Spacing();

    switch (Index)
    {
    case 0: // Visual
        ImGui::BeginChild("##C000", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 3.f), true);

        ImGui::Text("Visual");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("ESP", &g.g_ESP);
        ImGui::Checkbox("Item ESP", &g.g_ESP_Item);
        ImGui::Checkbox("Exfil ESP", &g.g_ESP_Exfil);
        ImGui::Checkbox("Grenade ESP", &g.g_ESP_Grenade);

        ImGui::EndChild();
        ImGui::BeginChild("##C001", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("ESP Options");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Line", &g.g_ESP_Line);
        ImGui::Checkbox("Box", &g.g_ESP_Box);
        ImGui::Checkbox("BoxFilled", &g.g_ESP_BoxFilled);
        ImGui::Checkbox("Skeleton", &g.g_ESP_Skeleton);
        ImGui::Checkbox("Distance", &g.g_ESP_Distance);
        ImGui::Checkbox("Name", &g.g_ESP_Name);
        ImGui::Checkbox("HealthBar", &g.g_ESP_HealthBar);

        ImGui::EndChild();
        break;
    case 1: // Misc
        ImGui::BeginChild("##C010", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 3.f), true);

        ImGui::Text("Misc");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("NoSway", &g.g_NoSway);
        ImGui::Checkbox("Inf. Stamina", &g.g_InfStamina);
        ImGui::Checkbox("No FallDamage", &g.g_NoFallDmg);
        ImGui::EndChild();
        
        ImGui::BeginChild("##C011", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::EndChild();
        break;
    case 2: // System
        ImGui::BeginChild("##C020", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 3.f), true);
        ImGui::Text("System");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("StreamProof", &g.g_StreamProof);

        ImGui::EndChild();
        ImGui::BeginChild("##C021", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("Crosshair");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Crosshair", &g.g_Crosshair);
        ImGui::CustomSliderInt("CrosshairSize", "##SizeCH", &g.g_CrosshairSize, 1, 10);
        ImGui::ColorEdit4("Color##C", &CrosshairColor.Value.x);
        ImGui::Combo("Type##C", &g.g_CrosshairType, CrosshairList, IM_ARRAYSIZE(CrosshairList));

        ImGui::EndChild();
        break;
    default:
        break;
    }

    ImGui::EndChild();
    //---------------------------------------------------//

    ImGui::SameLine();

    //---// Right //--------------------------------------//
    ImGui::BeginChild("##RightChild", ImVec2(ImGui::GetContentRegionAvail()), false);

    ImGui::Spacing();

    switch (Index)
    {
    case 0: // Visual
        ImGui::BeginChild("##C100", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 2.5f), true);

        ImGui::Text("ESP Setting");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::CustomSliderFloat("Player", "##Dist", &g.g_ESP_MaxDistance, 100.f, 2000.f);
        ImGui::CustomSliderFloat("Item", "##ItemDist", &g.g_ESP_MaxItemDistance, 10.f, 1000.f);

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Combo("BoxType", &g.g_ESP_BoxStyle, BoxTypeList, IM_ARRAYSIZE(BoxTypeList));

        ImGui::EndChild();
        ImGui::BeginChild("##C101", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("ESP Colors");
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::TreeNode("Player/Scav"))
        {
            ImGui::ColorEdit4("PMC", &Col_ESP_PMC.Value.x);
            ImGui::ColorEdit4("Scav", &Col_ESP_Scav.Value.x);
            ImGui::ColorEdit4("P-Scav", &Col_ESP_pScav.Value.x);
            ImGui::ColorEdit4("Scav++", &Col_ESP_SpecialScav.Value.x);
            ImGui::ColorEdit4("Boss", &Col_ESP_Boss.Value.x);
            ImGui::ColorEdit4("BoxFilled", &Col_ESP_Filled.Value.x);

            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Game"))
        {
            ImGui::Text("[+] Item");
            ImGui::ColorEdit4("Rare", &Col_ESP_RareItem.Value.x);
            
            ImGui::NewLine();
            ImGui::Spacing();

            ImGui::Text("[+] Exfil");
            ImGui::ColorEdit4("Open", &Col_ESP_ExfilOpen.Value.x);
            ImGui::ColorEdit4("Close", &Col_ESP_ExfilClose.Value.x);

            ImGui::TreePop();
        }

        ImGui::EndChild();
        break;
    case 1: // Misc
        ImGui::BeginChild("##C200", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::EndChild();
        break;
    case 2: // System
        ImGui::BeginChild("##C300", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("Exit");
        ImGui::Separator();
        ImGui::Spacing();
        if (ImGui::Button("Exit", ImVec2(ImGui::GetContentRegionAvail().x, 30.f)))
            g.process_active = false;

        ImGui::EndChild();
        break;
    default:
        break;
    }

    ImGui::EndChild();
    ImGui::EndChild();
    //---------------------------------------------------//

    ImGui::End();
}