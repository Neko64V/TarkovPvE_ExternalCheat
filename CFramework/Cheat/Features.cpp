#include "FrameCore.h"

// いくらかのリストを1RPMで読み取る為の構造体
struct CGameObjectList {
    uint64_t address[64]{};
};

// アイテムを1RPMで読み取るための構造体
struct CGameItemList {
    uint64_t address[2048]{};
};

void CFramework::MiscAll()
{
    uintptr_t LocalAddr = local.address;

    // BasePointer
    uintptr_t WeaponAnimation = pLocal->GetWeaponAnimation();
    uintptr_t Physics = m.Read<uintptr_t>(LocalAddr + offset::Physics);

    uintptr_t BreathEffector = m.Read<uintptr_t>(WeaponAnimation + 0x28);
    uintptr_t Stamina = m.Read<uintptr_t>(Physics + 0x38);

    // No Recoil/Sway
    if (g.g_NoSway) {
        m.Write<int>(WeaponAnimation + 0x138, 1);
        m.Write<float>(BreathEffector + 0xa4, 0.001f);
    }

    // InfStamina
    if (g.g_InfStamina && m.Read<float>(Stamina + 0x48) < 85.f)
        m.Write<float>(Stamina + 0x48, 90.f);

    // NoFall Damage
    if (g.g_NoFallDmg && m.Read<float>(Physics + 0xBC) != 0.f)
        m.Write<float>(Physics + 0xBC, 0.f);
}

void CFramework::UpdateList()
{
    // 仮のリスト用変数
    std::vector<CPlayer>    list_player{};
    std::vector<uintptr_t>  list_grenade{};

    while (g.process_active)
    {
        if (g.g_ESP && tarkov->Update())
        {
            // PlayerList
            const auto GameWorld = tarkov->GetLocalGameWorld();
            const auto registered_player = m.Read<uintptr_t>(GameWorld + offset::RegisteredPlayers);
            const auto entity_array = m.Read<UnityList>(registered_player);

            if (entity_array.count <= 0)
                continue;
            else if (entity_array.list_address == NULL)
                continue;

            auto local_addr = m.Read<uintptr_t>(GameWorld + offset::MainPlayer);

            if (pLocal->GetAddress(local_addr) && pLocal->Update())
            {
                pLocal->UpdateStatic();
                auto player_list = m.Read<CGameObjectList>(entity_array.list_address + 0x20);

                // ESP用EntityListを構築
                for (auto i = 0; i < entity_array.count; i++)
                {
                    CPlayer p;
                    if (p.GetAddress(player_list.address[i]) && player_list.address[i] != local_addr) // pointer is valid
                    {
                        // ここで距離のチェックとかしときたい
                        if (!Vec3_Empty(pLocal->m_vecLocation))
                            pLocal->Update();

                        float dist = GetDistance(pLocal->m_vecLocation, p.GetBonePosition(Base));

                        // Distance check
                        if (dist < g.g_ESP_MaxDistance) {
                            p.UpdateStatic();
                            p.m_iSpawnType = p.GetSpawnType();

                            list_player.push_back(p);
                        }
                    }
                }

                // GrenadeList
                if (g.g_ESP_Grenade)
                {
                    const auto grenade_class = m.Read<uintptr_t>(GameWorld + offset::GrenadeList);
                    const auto grenade_array_ptr = m.Read<uintptr_t>(grenade_class + 0x18);
                    const auto grenade_array = m.Read<UnityList>(grenade_array_ptr);

                    if (grenade_array.count > 0)
                    {
                        auto grenade_list = m.Read<CGameObjectList>(grenade_array.list_address + 0x20);

                        for (auto g = 0; g < grenade_array.count; g++)
                        {
                            if (grenade_list.address[g] != NULL) {
                                list_grenade.push_back(grenade_list.address[g]);
                            }  
                        }
                    }
                }
            }
        } 
        else {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            continue;
        }

        EntityList = list_player;
        GrenadeList = list_grenade;
        list_player.clear();
        list_grenade.clear();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

// 要改善
void CFramework::UpdateStaticList() // C6262 :(
{
    while (g.process_active)
    {
        std::vector<CItem>  list_item{};
        std::vector<CExfil>     list_exfil{};
        const auto GameWorld = tarkov->GetLocalGameWorld();

        // ItemList
        if (tarkov->Update())
        {
            // Exfil
            if (g.g_ESP_Exfil)
            {
                const auto exfil_controller = m.Read<uintptr_t>(GameWorld + offset::ExfilController);
                const auto exfil_array = m.Read<uintptr_t>(exfil_controller + 0x20);
                auto exfil_list = m.Read<CGameObjectList>(exfil_array + 0x20);

                for (auto j = 0; j < 24; j++)
                {
                    CExfil e;
                    if (e.GetAddress(exfil_list.address[j]))
                    {
                        if (!e.Update())
                            continue;

                        e.m_pExfilName = e.GetName();
                       
                        list_exfil.push_back(e);
                    }
                }
            }

            if (g.g_ESP_Item)
            {
                const auto LootList = m.Read<uintptr_t>(tarkov->GetLocalGameWorld() + offset::LootList);
                const auto ItemArray = m.Read<UnityList>(LootList);

                if (ItemArray.count != 0)
                {
                    auto item_list = m.Read<CGameItemList>(ItemArray.list_address + 0x20);

                    for (int k = 0; k < ItemArray.count; k++)
                    {
                        CItem item;

                        if (item.GetAddress(item_list.address[k])) {
                            item.Update();
                            list_item.push_back(item);
                        }
                    }
                }
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            continue;
        }

        ItemList = list_item;
        ExfilList = list_exfil;
        list_exfil.clear();
        list_item.clear();

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void CFramework::GetESPInfo(const int& SpawnType, std::string& vOutStr, ImColor& vOutColor)
{
    switch (SpawnType)
    {
    case SCAV:
        vOutStr = "Scav";
        vOutColor = Col_ESP_Scav;
        break;
    case SNIPER_SCAV:
        vOutStr = "Sniper";
        vOutColor = Col_ESP_Scav;
        break;
    case RESHALA_BOSS:
        vOutStr = "Reshala";
        vOutColor = Col_ESP_Boss;
        break;
    case RESHALA_FOLLOW:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case KILLA_BOSS:
        vOutStr = "Killa";
        vOutColor = Col_ESP_Boss;
        break;
    case SHTURMAN_BOSS:
        vOutStr = "Shturman";
        vOutColor = Col_ESP_Boss;
        break;
    case SHTURMAN_FOLLOW:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case GLUKHAR_BOSS:
        vOutStr = "Glukhar";
        vOutColor = Col_ESP_Boss;
        break;
    case GLUKHAR_FOLLOW_01:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case GLUKHAR_FOLLOW_02:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case GLUKHAR_FOLLOW_03:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case SANITOR_BOSS:
        vOutStr = "Sanitor";
        vOutColor = Col_ESP_Boss;
        break;
    case SANITOR_FOLLOW:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case NORMAL_SCAV:
        vOutStr = "Scav";
        vOutColor = Col_ESP_Scav;
        break;
    case CULTIST_01_SCAV:
        vOutStr = "Cultist";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case CULTIST_02_SCAV:
        vOutStr = "Cultist";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case TAGILLA_BOSS:
        vOutStr = "Tagilla";
        vOutColor = Col_ESP_Boss;
        break;
    case ROGUE_SCAV:
        vOutStr = "Rogue";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case ZRYACHIY_BOSS:
        vOutStr = "Zryachiy";
        vOutColor = Col_ESP_Boss;
        break;
    case ZRYACHIY_FOLLOW:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case KABAN_BOSS:
        vOutStr = "Kaban";
        vOutColor = Col_ESP_Boss;
        break;
    case KABAN_FOLLOW_0:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case KABAN_FOLLOW_1:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case KABAN_FOLLOW_2:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case KABAN_FOLLOW_3:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case KOLLONTAY_BOSS:
        vOutStr = "Kollontay";
        vOutColor = Col_ESP_Boss;
        break;
    case KOLLONTAY_FOLLOW_01:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case KOLLONTAY_FOLLOW_02:
        vOutStr = "follower";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case BTR_VEHICLE:
        vOutStr = "BTR-82";
        vOutColor = ImColor(1.f, 1.f, 1.f, 1.f);
        break;
    case SCAV_PARTISAN:
        vOutStr = "Partisan";
        vOutColor = Col_ESP_Scav;
        break;
    case RAIDER_SCAV:
        vOutStr = "Raider";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case KNIGHT_SCAV:
        vOutStr = "Knight";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case BIGPIPE_SCAV:
        vOutStr = "BigPipe";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case BRIDEYE_SCAV:
        vOutStr = "Birdeye";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case BLOODHOUND_SCAV:
        vOutStr = "Bloodhound";
        vOutColor = Col_ESP_SpecialScav;
        break;
    case PMC_BEAR_PvE:
        vOutStr = "BEAR";
        vOutColor = Col_ESP_PMC;
        break;
    case PMC_USEC_PvE:
        vOutStr = "USEC";
        vOutColor = Col_ESP_PMC;
        break;
    case PMC_BEAR_NEW:
        vOutStr = "BEAR";
        vOutColor = Col_ESP_PMC;
        break;
    case PMC_USEC_NEW:
        vOutStr = "USEC";
        vOutColor = Col_ESP_PMC;
        break;
    default:
        vOutStr = "InValid";
        vOutColor = ImColor(1.f, 1.f, 1.f, 1.f);
        break;
    };
}

CFramework::~CFramework()
{
    delete tarkov;
}