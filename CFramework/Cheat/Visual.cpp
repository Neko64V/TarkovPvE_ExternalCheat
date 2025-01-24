#include "FrameCore.h"

void CFramework::RenderInfo()
{
    // ウォーターマーク
    std::string InfoText = "Tarkov PvE | " + std::to_string((int)ImGui::GetIO().Framerate) + "FPS";
    StringEx(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), InfoText.c_str());

    // Crosshair
    if (g.g_Crosshair && !local.IsAiming())
    {
        switch (g.g_CrosshairType)
        {
        case 0: {
            ImVec2 Center = ImVec2(g.GameRect.right / 2, g.GameRect.bottom / 2);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x - g.g_CrosshairSize, Center.y), ImVec2((Center.x + g.g_CrosshairSize) + 1, Center.y), CrosshairColor, 1);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x, Center.y - g.g_CrosshairSize), ImVec2(Center.x, (Center.y + g.g_CrosshairSize) + 1), CrosshairColor, 1);
        }   break;
        case 1:
            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.g_CrosshairSize, CrosshairColor, NULL);
            //ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.g_CrosshairSize + 1, ImColor(0.f, 0.f, 0.f, 1.f), NULL);
            //ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.g_CrosshairSize, CrosshairColor, NULL);
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

    // コピー用変数
    std::vector<CPlayer>    list_player = EntityList;
    std::vector<CExfil>     list_exfil  = ExfilList;
    std::vector<CItem>      list_item   = ItemList;
    std::vector<uintptr_t>  list_grenade= GrenadeList;
    
    // るーぷするよ
    for (auto& entity : list_player)
    {
        CPlayer* pEntity = &entity;

        if (!pEntity->Update())
            continue;
        
        // 距離を算出
        float Distance = GetDistance(pLocal->m_vecLocation, pEntity->m_vecLocation);

        // 範囲外のプレイヤーは除外
        if (g.g_ESP_MaxDistance < Distance)
            continue;

        // WorldToScreen
        Vector2 pScreen{}, pHead{}, pNeck{};;
        if (!WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), pEntity->m_vecLocation, pScreen))
            continue;

        // WorldToScreen
        pEntity->UpdateBone();
        if (!WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), pEntity->m_vecHeadLocation, pHead) ||
            !WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), pEntity->m_vecNeckLocation, pNeck))
            continue;

        // 各種サイズの算出等
        const float HeadToNeck = pNeck.y - pHead.y;
        const float pHeight = pScreen.y - pHead.y + (HeadToNeck * 2.5f);
        const float pWidth  = pHeight / 3.5f;

        std::string Name = "InValid";
        ImColor pColor = ImColor(1.f, 1.f, 1.f, 1.f);
        GetESPInfo(pEntity->m_iSpawnType, Name, pColor);

        // BTR-82
        if (pEntity->m_iSpawnType == BTR_VEHICLE) {
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(pScreen.x, pScreen.y - 2.f), 2.f, ImColor(1.f, 1.f, 1.f, 1.f), 0.f);

            std::string BTR_Text = "BTR-82 [" + std::to_string((int)Distance) + "m]";
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
        if (g.g_ESP_Skeleton && 350.f > Distance) // パフォーマンスの都合上400m以上先のプレイヤーはスキップ
        {
            // BoneList
            Vector3 list[][2] = { 
                { pEntity->m_vecNeckLocation, pEntity->m_pVecBoneList[CPelvis] }, 
                { pEntity->m_vecNeckLocation, pEntity->m_pVecBoneList[CLeftForearm] },
                { pEntity->m_pVecBoneList[CLeftForearm], pEntity->m_pVecBoneList[CLeftPalm] },
                { pEntity->m_vecNeckLocation, pEntity->m_pVecBoneList[CRightForearm] },
                { pEntity->m_pVecBoneList[CRightForearm], pEntity->m_pVecBoneList[CRightPalm] },
                { pEntity->m_pVecBoneList[CPelvis], pEntity->m_pVecBoneList[CLeftThigh] },
                { pEntity->m_pVecBoneList[CLeftThigh], pEntity->m_pVecBoneList[CLeftFoot] },
                { pEntity->m_pVecBoneList[CPelvis], pEntity->m_pVecBoneList[CRightThigh] },
                { pEntity->m_pVecBoneList[CRightThigh], pEntity->m_pVecBoneList[CRightFoot] }
            };

            // Body
            for (int j = 0; j < 9; j++) {
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
        if (g.g_ESP_HealthBar) {
            pEntity->UpdateHealth();
            HealthBar(pScreen.x - pWidth - 4.f, pScreen.y, 2.f, -pHeight, pEntity->m_fHealth, pEntity->m_fHealthMax);
        }
        
        // Name
        if (g.g_ESP_Name && pEntity->m_iSpawnType != SCAV && pEntity->m_iSpawnType != SNIPER_SCAV && pEntity->m_iSpawnType != NORMAL_SCAV)
        {
            // 不明なSpawnTypeだったら値を表示
            if (!Name.compare("InValid"))
                Name += "[" + std::to_string(pEntity->m_iSpawnType) + "]";

            String(ImVec2(pScreen.x - (ImGui::CalcTextSize(Name.c_str()).x / 2.f), pScreen.y - pHeight - 14.f), ImColor(1.f, 1.f, 1.f, 1.f), Name.c_str());
        }
    }

    // Item
    if (g.g_ESP_Item)
    {
        for (auto& item : list_item)
        {
            CItem* pItem = &item;

            // Filter
            if (pItem->m_iName.empty())
                continue;

            float ItemDistance = GetDistance(pLocal->m_vecLocation, pItem->m_pVecLocation);

            // Distance Check
            if (ItemDistance > g.g_ESP_MaxItemDistance)
                continue;

            Vector2 pItemScreen{};
            if (!WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), pItem->m_pVecLocation, pItemScreen))
                continue;

            std::string vItemTx = pItem->m_iName + "[" + std::to_string((int)ItemDistance) + "m]";
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(pItemScreen.x, pItemScreen.y - 2.f), 2.f, Col_ESP_RareItem, 0.f);
            String(ImVec2(pItemScreen.x - (ImGui::CalcTextSize(vItemTx.c_str()).x / 2.f), pItemScreen.y), Col_ESP_RareItem, vItemTx.c_str());
        }
    }

    // Grenade
    if (g.g_ESP_Grenade)
    {
        for (auto& grenade : GrenadeList)
        {
            uintptr_t TransformInternal = m.ReadChain(grenade, { 0x10, 0x30, 0x30, 0x8, 0x28, 0x10 });
            Vector3 m_pVecLocation = GetTransformPosition(TransformInternal);

            if (Vec3_Empty(m_pVecLocation))
                continue;

            float gDistance = GetDistance(pLocal->m_vecLocation, m_pVecLocation);

            if (gDistance > 100.f)
                continue;
            else if (gDistance < 10.f)
                String(ImVec2(g.GameRect.right / 2.f - (ImGui::CalcTextSize("[ WARNING ] Grenade!!").x / 2.f), g.GameRect.bottom / 2.f), ImColor(1.f, 0.f, 0.f, 1.f), "[ WARNING ] Grenade!!");


            Vector2 pGrenadeRoot{};
            if (!WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), m_pVecLocation, pGrenadeRoot))
                continue;

            std::string gre_tx = "Grenade [" + std::to_string(int(gDistance)) + "m]";
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(pGrenadeRoot.x, pGrenadeRoot.y), 2.f, ImColor(1.f, 0.f, 0.f, 1.f), 0.f);
            String(ImVec2(pGrenadeRoot.x - (ImGui::CalcTextSize(gre_tx.c_str()).x / 2.f), pGrenadeRoot.y - 13.f), ImColor(1.f, 0.f, 0.f, 1.f), gre_tx.c_str());
        }
    }

    // Exfil
    if (g.g_ESP_Exfil)
    {
        for (auto& exfil : ExfilList)
        {
            CExfil* pExfil = &exfil;

            Vector2 pExfilScreen{};
            if (!WorldToScreen(ViewMatrix, Vector2(g.GameRect.right, g.GameRect.bottom), pExfil->m_pVecLocation, pExfilScreen))
                continue;

            ImColor ExfilColor = pExfil->m_pExfilStatus == NOTREADY ? Col_ESP_ExfilClose : Col_ESP_ExfilOpen;

            std::string exfilText = pExfil->m_pExfilName + " [" + std::to_string((int)GetDistance(pLocal->m_vecLocation, pExfil->m_pVecLocation)) + "m]";

            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(pExfilScreen.x, pExfilScreen.y - 2.f), 2.f, ExfilColor, 0.f);
            String(ImVec2(pExfilScreen.x - (ImGui::CalcTextSize(exfilText.c_str()).x / 2.f), pExfilScreen.y), ExfilColor, exfilText.c_str());
        }
    }
}