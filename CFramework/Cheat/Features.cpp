#include "FrameCore.h"

void CFramework::UpdateList()
{
    while (g.Run)
    {
        std::vector<CPlayer> _playerlist{};
        std::vector<CExfil> _exfillist{};

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

            // ESPópEntityListÇç\íz
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
        }

        EntityList = _playerlist;
        ExfilList = _exfillist;
        _playerlist.clear();
        _exfillist.clear();

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
    case ASSAULT_SCAV:
        vOutStr = "Scav++";
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