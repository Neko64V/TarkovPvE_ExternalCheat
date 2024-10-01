#include "Cheat.h"

void Cheat::UpdateList()
{
    while (g.Run)
    {
        std::vector<CPlayer> TempPlayerList;
        std::vector<CItem> TempItemList;
        std::vector<CExfil> TempExfilList;
        std::vector<uintptr_t> nadeList;

        if (EFT.InitAddress())
        {
            // Baseptr
            uintptr_t RegisteredPlayer = m.Read<uintptr_t>(EFT.localGameWorld + Offsets::LocalGameWorld::RegisteredPlayers);
            UnityList PlayerArray = m.Read<UnityList>(RegisteredPlayer + 0x10);

            if (!PlayerArray.Base || PlayerArray.Count <= 0) {
                Sleep(10);
                continue;
            }

            uintptr_t lp = m.Read<uintptr_t>(PlayerArray.Base + 0x20);
            if (!local.GetPlayer(lp)) {
                Sleep(10);
                continue;
            }

            if (!local.Update()) {
                Sleep(10);
                continue;
            }

            ItemCount = 0;

            for (int i = 1; i < PlayerArray.Count; i++)
            {
                CPlayer player{};
                uintptr_t p_entity = m.Read<uintptr_t>(PlayerArray.Base + 0x20 + (i * 0x8));

                if (!player.GetPlayer(p_entity))
                    continue;

                TempPlayerList.push_back(player);
            }

            // Item
            uintptr_t LootList = m.Read<uintptr_t>(EFT.localGameWorld + Offsets::LocalGameWorld::LootList);
            UnityList ItemArray = m.Read<UnityList>(LootList + 0x10);

            for (int j = 0; j < ItemArray.Count; j++)
            {
                CItem item{};
                uintptr_t i_entity = m.Read<uintptr_t>(ItemArray.Base + 0x20 + (j * 0x8));

                if (!item.GetEntity(i_entity))
                    continue;
                else if (!item.Update())
                    continue;

                TempItemList.push_back(item);
            }

            // Exfil
            uintptr_t ExfilController = m.Read<uintptr_t>(EFT.localGameWorld + Offsets::LocalGameWorld::ExfilController);
            uintptr_t ExfilArray = m.Read<uintptr_t>(ExfilController + 0x20);

            for (int k = 0; k < 32; k++)
            {
                CExfil exfil{};
                uintptr_t e_entity = m.Read<uintptr_t>(ExfilArray + 0x20 + (k * 0x8));

                if (!exfil.GetEntity(e_entity))
                    break;
                else if (!exfil.Update())
                    continue;

                TempExfilList.push_back(exfil);
            }

            ItemCount = ItemArray.Count;
        }
        else
        {
            Sleep(10);
            continue;
        }

        EntityList = TempPlayerList;
        ExfilList = TempExfilList;
        ItemList = TempItemList;

        TempPlayerList.clear();
        TempExfilList.clear();
        TempItemList.clear();
        nadeList.clear();

        Sleep(500);
    }
}

void Cheat::Misc()
{
    while (g.Run)
    {
        uintptr_t WeaponAnimation = m.Read<uintptr_t>(local.ptr + 0x1C0);

        //Vector3 shotDirection = m.Read<Vector3>(WeaponAnimation + 0x224);
        //printf("%.2f, %.2f, %.2f\n", shotDirection.x, shotDirection.y, shotDirection.z);

        uintptr_t Physics = m.Read<uintptr_t>(local.ptr + 0x5d0);

        uintptr_t BreathEffector = m.Read<uintptr_t>(WeaponAnimation + 0x28);
        uintptr_t Stamina = m.Read<uintptr_t>(Physics + 0x38);

        // NoRecoil/NoSway
        if (g.NoSway)
        {
            m.Write<int>(WeaponAnimation + 0x138, 1);
            m.Write<float>(BreathEffector + 0xa4, 0.001f);
        }

        // InfStamina
        if (g.InfStamina && m.Read<float>(Stamina + 0x48) < 85.f)
            m.Write<float>(Stamina + 0x48, 90.f);

        // No FallDamage
        if (g.NoFallDamage && m.Read<float>(Physics + 0xbc) != 0.f)
            m.Write<float>(Physics + 0xbc, 0.f);

        Sleep(100);
    }
}

ImColor Cheat::SetColor(int SpawnType)
{
    ImColor color = { 1.f, 1.f, 1.f, 1.f };

    switch (SpawnType)
    {
    case Scav:
        return Col_ESP_Scav;
    case Scav_Sniper:
        return Col_ESP_Scav;
    case Boss_Reshala:
        return Col_ESP_Boss;
    case Follower_Reshala:
        return Col_ESP_SpecialScav;
    case Boss_Killa:
        return Col_ESP_Boss;
    case Boss_Shturman:
        return Col_ESP_Boss;
    case Follower_Shturman:
        return Col_ESP_SpecialScav;
    case Boss_Glukhar:
        return Col_ESP_Boss;
    case Follower_Glukhar01:
        return Col_ESP_SpecialScav;
    case Follower_Glukhar02:
        return Col_ESP_SpecialScav;
    case Follower_Glukhar03:
        return Col_ESP_SpecialScav;
    case Boss_Sanitar:
        return Col_ESP_Boss;
    case Follower_Sanitar:
        return Col_ESP_SpecialScav;
    case Cultist01:
        return Col_ESP_SpecialScav;
    case Cultist02:
        return Col_ESP_SpecialScav;
    case Boss_Tagilla:
        return Col_ESP_Boss;
    case Scav_Rogue:
        return Col_ESP_SpecialScav;
    case Scav_Zryachiy:
        return Col_ESP_Boss;
    case Follower_Zryachiy:
        return Col_ESP_SpecialScav;
    case Boss_Kaban:
        return Col_ESP_Boss;
    case Follower_Kaban:
        return Col_ESP_SpecialScav;
    case Boss_Kollontay:
        return Col_ESP_Boss;
    case Follower_Kollontay01:
        return Col_ESP_SpecialScav;
    case Follower_Kollontay02:
        return Col_ESP_SpecialScav;
    case Vehicle_BTR:
        return color;
    case Scav_Raider:
        return Col_ESP_SpecialScav;
    case Scav_Knight:
        return Col_ESP_SpecialScav;
    case Scav_BigPipe:
        return Col_ESP_SpecialScav;
    case Scav_Brideye:
        return Col_ESP_SpecialScav;
    case Bloodhound:
        return Col_ESP_SpecialScav;
    case PMC_USEC_Old:
        return Col_ESP_PMC;
    case PMC_BEAR_Old:
        return Col_ESP_PMC;
    case PMC_BEAR_PvE:
        return Col_ESP_PMC;
    case PMC_USEC_PvE:
        return Col_ESP_PMC;
    default:
        return color;
    };
}