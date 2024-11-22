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

    // **SilentAim** に使えそう
    //Vector3 shotDirection = m.Read<Vector3>(WeaponAnimation + 0x224);
}

void CFramework::UpdateList()
{
    while (g.Run)
    {
        std::vector<CPlayer> _playerlist{};
        std::vector<CExfil> _exfillist{};
        //std::vector<CItem> _itemlist{};

        if (g.g_ESP && tarkov->Update())
        {
            // Get EntityList
            uintptr_t RegisteredPlayer = m.Read<uintptr_t>(tarkov->m_LocalGameWorld + offset::RegisteredPlayers);
            UnityList _entitylist = m.Read<UnityList>(RegisteredPlayer + 0x10);

            if (_entitylist.ptr == 0 || _entitylist.count <= 0) {
                Sleep(500);
                continue;
            } 

            // Get Local
            uintptr_t _local = m.Read<uintptr_t>(_entitylist.ptr + 0x20);

            if (!local.GetEntity(_local) || !local.Update()) {
                Sleep(500);
                continue;
            }

            // ESP用EntityListを構築
            for (int i = 1; i < _entitylist.count; i++)
            {
                CPlayer player{};
                uintptr_t _entity = m.Read<uintptr_t>(_entitylist.ptr + 0x20 + (i * 0x8));

                if (!player.GetEntity(_entity))
                    continue;

                _playerlist.push_back(player);
            }

            // Exfil
            if (g.g_ExfilESP)
            {
                uintptr_t ExfilController = m.Read<uintptr_t>(tarkov->m_LocalGameWorld + offset::ExfilController);
                uintptr_t ExfilArray = m.Read<uintptr_t>(ExfilController + 0x20);

                for (int j = 0; j < 16; j++)
                {
                    CExfil exfil{};
                    uintptr_t _exfil_addr = m.Read<uintptr_t>(ExfilArray + 0x20 + (j * 0x8));

                    if (!exfil.GetExfil(_exfil_addr) || !exfil.Update())
                        break;

                    _exfillist.push_back(exfil);
                }
            }

            // Item
            /*
            uintptr_t LootList = m.Read<uintptr_t>(tarkov->m_LocalGameWorld + offset::LootList);
            UnityList ItemArray = m.Read<UnityList>(LootList + 0x10);

            for (int k = 0; k < ItemArray.count; k++)
            {
                CItem item{};
                uintptr_t i_entity = m.Read<uintptr_t>(ItemArray.ptr + 0x20 + (k * 0x8));

                if (!item.GetItem(i_entity) || !item.Update())
                    continue;

                _itemlist.push_back(item);
            }*/
        }

        EntityList = _playerlist;
        ExfilList = _exfillist;
        //ItemList = _itemlist;
        _playerlist.clear();
        _exfillist.clear();
        //_itemlist.clear();

        Sleep(1000);
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
    case KABAN_FOLLOW:
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
    case PMC_USEC_Old:
        vOutStr = "USEC";
        vOutColor = Col_ESP_PMC;
        break;
    case PMC_BEAR_Old:
        vOutStr = "BEAR";
        vOutColor = Col_ESP_PMC;
        break;
    case PMC_BEAR_PvE:
        vOutStr = "BEAR";
        vOutColor = Col_ESP_PMC;
        break;
    case PMC_USEC_PvE:
        vOutStr = "USEC";
        vOutColor = Col_ESP_PMC;
        break;
    case ASSAULT_SCAV_RU:
        vOutStr = "Scav+";
        vOutColor = Col_ESP_Scav;
        break;
    case ASSAULT_SCAV_UN:
        vOutStr = "Scav+";
        vOutColor = Col_ESP_Scav;
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