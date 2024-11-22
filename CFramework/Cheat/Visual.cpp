#include "FrameCore.h"

void CFramework::RenderInfo()
{
    // ウォーターマーク
    //StringEx(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), "Tarkov PvE");

    std::string fps_t = std::to_string(ImGui::GetIO().Framerate) + "FPS";
    StringEx(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), fps_t.c_str());

    // Crosshair
    if (g.g_Crosshair)
    {
        switch (g.g_CrosshairType)
        {
        case 0: {
            ImVec2 Center = ImVec2(g.GameRect.right / 2, g.GameRect.bottom / 2);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x - g.g_CrosshairSize, Center.y), ImVec2((Center.x + g.g_CrosshairSize) + 1, Center.y), CrosshairColor, 1);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x, Center.y - g.g_CrosshairSize), ImVec2(Center.x, (Center.y + g.g_CrosshairSize) + 1), CrosshairColor, 1);
        }   break;
        case 1:
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.g_CrosshairSize + 1, ImColor(0.f, 0.f, 0.f, 1.f), NULL);
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.g_CrosshairSize, CrosshairColor, NULL);
            break;
        }
    }
}

void CFramework::RenderESP()
{
    // Localの更新
    CPlayer* pLocal = &local;
   
    if (!pLocal->Update())
        return;

    // ViewMatrix
    tarkov->UpdateCamera();
    Matrix ViewMatrix = tarkov->GetViewMatrix(); // WorldToScreenの度に呼び出す必要はない

    std::vector<CPlayer> list = EntityList;

    // るーぷするよ
    for (auto& entity : list)
    {
        CPlayer* pEntity = &entity;

        if (!pEntity->Update())
            continue;

        // 距離を算出
        float Distance = GetDistance(pLocal->m_pVecLocation, pEntity->m_pVecLocation);

        // 範囲外のプレイヤーは除外
        if (g.g_ESP_MaxDistance < Distance)
            continue;

        // WorldToScreen
        Vector2 pScreen{}, pHead{}, pNeck{};
        if (!WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), pEntity->m_pVecLocation, pScreen) ||
            !WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), pEntity->m_pBoneList[1], pHead) || 
            !WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), pEntity->m_pBoneList[2], pNeck))
            continue;

        // 各種サイズの算出等
        const float HeadToNeck = pNeck.y - pHead.y;
        const float pHeight = pScreen.y - pHead.y + (HeadToNeck * 2.5f);
        const float pWidth  = pHeight / 3.5f;

        std::string Name = "InValid";
        ImColor pColor = ImColor(1.f, 1.f, 1.f, 1.f);
        GetESPInfo(pEntity->m_pSpawnType, Name, pColor);

        // BTR-82
        if (pEntity->m_pSpawnType == BTR_VEHICLE) {
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(pScreen.x, pScreen.y - 2.f), 2.f, ImColor(1.f, 1.f, 1.f, 1.f), 0.f);

            std::string BTR_Text = "BTR-82[" + std::to_string((int)Distance) + "m]";
            String(ImVec2(pScreen.x - (ImGui::CalcTextSize(BTR_Text.c_str()).x / 2.f), pScreen.y), ImColor(1.f, 1.f, 1.f, 1.f), BTR_Text.c_str());

            continue;
        }

        // Line
        if (g.g_ESP_Line)
            DrawLine(ImVec2(g.GameRect.right / 2.f, g.GameRect.bottom), ImVec2(pScreen.x, pScreen.y), pColor, 1.f);

        // 2D Box
        if (g.g_ESP_Box)
        {
            static float bScale = 1.75f;

            // Filled
            if (g.g_ESP_BoxFilled)
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(pScreen.x - pWidth, pScreen.y - pHeight), ImVec2(pScreen.x + pWidth, pScreen.y), Col_ESP_Filled, 1.f);

            // Box
            switch (g.g_ESP_BoxStyle)
            {
            case 0:
                DrawLine(ImVec2(pScreen.x - pWidth, pScreen.y - pHeight), ImVec2(pScreen.x + pWidth, pScreen.y - pHeight), pColor, 1.f);
                DrawLine(ImVec2(pScreen.x - pWidth, pScreen.y), ImVec2(pScreen.x - pWidth, pScreen.y - pHeight), pColor, 1.f);
                DrawLine(ImVec2(pScreen.x + pWidth, pScreen.y), ImVec2(pScreen.x + pWidth, pScreen.y - pHeight), pColor, 1.f);
                DrawLine(ImVec2(pScreen.x - pWidth, pScreen.y), ImVec2(pScreen.x + pWidth, pScreen.y), pColor, 1.f);
                break;
            case 1:
                DrawLine(ImVec2(pScreen.x - pWidth, pScreen.y - pHeight), ImVec2(pScreen.x - pWidth + (pWidth / bScale), pScreen.y - pHeight), pColor, 1.f);
                DrawLine(ImVec2(pScreen.x + pWidth, pScreen.y - pHeight), ImVec2(pScreen.x + pWidth - (pWidth / bScale), pScreen.y - pHeight), pColor, 1.f);
                DrawLine(ImVec2(pScreen.x - pWidth, pScreen.y - pHeight), ImVec2(pScreen.x - pWidth, pScreen.y - pHeight + (pWidth / bScale)), pColor, 1.f);
                DrawLine(ImVec2(pScreen.x + pWidth, pScreen.y - pHeight), ImVec2(pScreen.x + pWidth, pScreen.y - pHeight + (pWidth / bScale)), pColor, 1.f);
                DrawLine(ImVec2(pScreen.x - pWidth, pScreen.y), ImVec2(pScreen.x - pWidth, pScreen.y - (pWidth / bScale)), pColor, 1.f);
                DrawLine(ImVec2(pScreen.x + pWidth, pScreen.y), ImVec2(pScreen.x + pWidth, pScreen.y - (pWidth / bScale)), pColor, 1.f);
                DrawLine(ImVec2(pScreen.x - pWidth, pScreen.y), ImVec2(pScreen.x - pWidth + (pWidth / bScale), pScreen.y), pColor, 1.f);
                DrawLine(ImVec2(pScreen.x + pWidth, pScreen.y), ImVec2(pScreen.x + pWidth - (pWidth / bScale), pScreen.y), pColor, 1.f);
                break;
            default:
                break;
            }
        }

        // Skeleton - 上のUpdate()で必要なBoneを全て読み取りここで再構築を行う。少し複雑。
        if (g.g_ESP_Skeleton && 400.f > Distance) // パフォーマンスの都合上400m以上先のプレイヤーはスキップ
        {
            // BoneList
            Vector3 list[][2] = { 
                { pEntity->m_pBoneList[CNeck], pEntity->m_pBoneList[CPelvis] }, 
                { pEntity->m_pBoneList[CNeck], pEntity->m_pBoneList[CLeftForearm] },
                { pEntity->m_pBoneList[CLeftForearm], pEntity->m_pBoneList[CLeftPalm] },
                { pEntity->m_pBoneList[CNeck], pEntity->m_pBoneList[CRightForearm] },
                { pEntity->m_pBoneList[CRightForearm], pEntity->m_pBoneList[CRightPalm] },
                { pEntity->m_pBoneList[CPelvis], pEntity->m_pBoneList[CLeftThigh] },
                { pEntity->m_pBoneList[CLeftThigh], pEntity->m_pBoneList[CLeftFoot] },
                { pEntity->m_pBoneList[CPelvis], pEntity->m_pBoneList[CRightThigh] },
                { pEntity->m_pBoneList[CRightThigh], pEntity->m_pBoneList[CRightFoot] }
            };

            // Body
            for (int j = 0; j < 9; j++)
            {
                Vector2 bOut1{}, bOut2{};
                if (!WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), list[j][0], bOut1) || !WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), list[j][1], bOut2))
                    break;

                DrawLine(ImVec2(bOut1.x, bOut1.y), ImVec2(bOut2.x, bOut2.y), pColor, 1.f);
            }

            // Head Circle
            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(pHead.x, pHead.y - (HeadToNeck / 2.f)), HeadToNeck * 1.55f, pColor, 0.f);
        }

        // Distance
        if (g.g_ESP_Distance) {
            std::string DistanceStr = std::to_string((int)Distance) + "m";
            String(ImVec2(pScreen.x - (ImGui::CalcTextSize(DistanceStr.c_str()).x / 2.f), pScreen.y), ImColor(1.f, 1.f, 1.f, 1.f), DistanceStr.c_str());
        }

        // Healthbar
        if (g.g_ESP_HealthBar)
            HealthBar(pScreen.x - pWidth - 4.f, pScreen.y, 2.f, -pHeight, pEntity->m_pHealth, pEntity->m_pHealthMax);

        // Name
        if (g.g_ESP_Name && pEntity->m_pSpawnType != SCAV && pEntity->m_pSpawnType != SNIPER_SCAV)
        {
            if (!Name.compare("InValid"))
                Name += "[" + std::to_string(pEntity->m_pSpawnType) + "]";
            String(ImVec2(pScreen.x - (ImGui::CalcTextSize(Name.c_str()).x / 2.f), pScreen.y - pHeight - 14.f), ImColor(1.f, 1.f, 1.f, 1.f), Name.c_str());
        }
    }
}