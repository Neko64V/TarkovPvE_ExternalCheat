#include "FrameCore.h"

void CFramework::MiscAll()
{
    uintptr_t LocalAddr = local.ptr;

    // BasePointer
    uintptr_t WeaponAnimation = m.Read<uintptr_t>(LocalAddr + offset::WeaponAnimation);
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
            const auto registered_player = m.Read<uintptr_t>(tarkov->GetLocalGameWorld() + offset::RegisteredPlayers);
            const auto entity_array = m.Read<UnityList>(registered_player);

            if (entity_array.count <= 0)
                continue;
            else if (entity_array.ctx == 0)
                continue;

            auto local_addr = m.Read<uintptr_t>(tarkov->GetLocalGameWorld() + offset::MainPlayer);
            
            if (local.GetEntity(local_addr) && local.Update())
            {
                // ESP用EntityListを構築
                for (auto ent = 0; ent < entity_array.count; ent++)
                {
                    CPlayer player{};
                    auto player_addr = m.Read<uintptr_t>(entity_array.ctx + 0x20 + (ent * 0x8));

                    if (player_addr == NULL || local_addr == player_addr)
                        continue;

                    if (player.GetEntity(player_addr))
                        list_player.push_back(player);
                }
            }

            // GrenadeList
            if (g.g_ESP_Grenade)
            {
                const auto grenade_class = m.Read<uintptr_t>(tarkov->GetLocalGameWorld() + offset::GrenadeList);
                const auto grenade_array_ptr = m.Read<uintptr_t>(grenade_class + 0x18);
                const auto grenade_array = m.Read<UnityList>(grenade_array_ptr);

                for (auto g = 0; g < grenade_array.count; g++)
                {
                    uintptr_t grenade_addr = m.Read<uintptr_t>(grenade_array.ctx + 0x20 + (g * 0x8));

                    if (grenade_addr != NULL)
                        list_grenade.push_back(grenade_addr);
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

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void CFramework::UpdateStaticList()
{
    while (g.process_active)
    {
        std::vector<CItem>  list_item{};
        std::vector<CExfil> list_exfil{};
        static int OldSize = 0;

        if (tarkov->Update())
        {
            // ItemList
            if (g.g_ESP_Item)
            {
                const auto LootList = m.Read<uintptr_t>(tarkov->GetLocalGameWorld() + offset::LootList);
                const auto ItemArray = m.Read<UnityList>(LootList);

                if (ItemArray.count != OldSize)
                {
                    for (int k = 0; k < ItemArray.count; k++)
                    {
                        CItem item{};
                        auto i_entity = m.Read<uintptr_t>(ItemArray.ctx + 0x20 + (k * 0x8));

                        if (!item.GetItem(i_entity) || !item.Update())
                            continue;

                        list_item.push_back(item);
                    }
                }

                OldSize = ItemArray.count;
            }
            
            // Exfil
            if (g.g_ESP_Exfil)
            {
                const auto exfil_controller = m.Read<uintptr_t>(tarkov->GetLocalGameWorld() + offset::ExfilController);
                const auto exfil_array = m.Read<uintptr_t>(exfil_controller + 0x20);

                for (auto j = 0; j < 12; j++)
                {
                    CExfil exfil{};
                    auto exfil_addr = m.Read<uintptr_t>(exfil_array + 0x20 + (j * 0x8));

                    if (exfil.GetExfil(exfil_addr) && exfil.Update()) {
                        list_exfil.push_back(exfil);
                    }
                    else {
                        break;
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

        list_item.clear();
        list_exfil.clear();

        std::this_thread::sleep_for(std::chrono::seconds(15));
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