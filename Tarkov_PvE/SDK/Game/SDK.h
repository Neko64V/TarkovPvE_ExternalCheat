#pragma once
#include "Struct.h"
#include "..\..\Utils\Memory\Memory.h"
#include "..\..\Utils\Config\Config.h"

namespace Offsets
{
    constexpr auto GameObjectManager = 0x17FFD28;

    namespace LocalGameWorld 
    {
        constexpr auto ExfilController   = 0x18;
        constexpr auto LootList          = 0xC8;
        constexpr auto RegisteredPlayers = 0xF0;
    }
    namespace Player
    {
        constexpr auto Profile = 0x5C0;
        constexpr auto HealthController = 0x600;
    }
    namespace Profile // EFT.Profile
    {
        constexpr auto Id = 0x10;
        constexpr auto AccountId = 0x18; 
        constexpr auto PlayerInfo = 0x28; 
        constexpr auto Stats = 0xF8; 
    }
    namespace PlayerInfo
    {
        constexpr auto Nickname = 0x10;
        constexpr auto MainProfileNickname = 0x18;
        constexpr auto GroupId = 0x20; // str 
		constexpr auto TeamId = 0x28; // str
        constexpr auto Settings = 0x50; 
        constexpr auto PlayerSide = 0xE8;
    }
}

class Tarkov
{
private:
    bool GetCamera();
    uintptr_t GetObjectFromList(uintptr_t listPtr, uintptr_t lastObjectPtr, const char* objectName);
public:
    // EFT Data
	GameObjectManager GOM{};
    uintptr_t GameWorld = 0;
    uintptr_t localGameWorld = 0;
    uintptr_t fpsCamera = 0;
	Matrix    ViewMatrix{};

    // SomeFunctions
    bool InitAddress();
    bool WorldToScreen(const Vector3& vIn, Vector2& vOut);
};

enum WildSpanwType : int
{
	// �ʏ�Scav
	Scav_Sniper = 0,
	Scav = 1,

	// Customs
	Boss_Reshala = 3,
	Follower_Reshala = 5,

	// InterChange
	Boss_Killa = 6,

	// Woods - �m��
	Boss_Shturman = 7,
	Follower_Shturman = 8,

	// Reserve
	Boss_Glukhar = 11,
	Follower_Glukhar01 = 12,
	Follower_Glukhar02 = 13,
	Follower_Glukhar03 = 14,

	// Shoreline
	Follower_Sanitar = 16,
	Boss_Sanitar = 17,

	// Factory
	Boss_Tagilla = 22,

	// Lighthouse
	Scav_Rogue = 24,
	Scav_Zryachiy = 29,
	Follower_Zryachiy = 30,

	// Street of Tarkov
	Boss_Kaban = 32,
	Follower_Kaban = 36,
	Boss_Kollontay = 43,
	Follower_Kollontay01 = 44,
	Follower_Kollontay02 = 45,
	Vehicle_BTR = 46,

	// Global
	Scav_Raider = 9,
	Scav_Assault = 19,
	Cultist01 = 20,
	Cultist02 = 21, // ��q?
	Scav_Knight = 26,
	Scav_BigPipe = 27,
	Scav_Brideye = 28,
	Bloodhound = 35,

	// Player
	PMC_USEC_Old = 47,
	PMC_BEAR_Old = 48,
	PMC_BEAR_PvE = 49,
	PMC_USEC_PvE = 50,

	// 19 : NoamralScav?
};

extern Tarkov EFT;
extern bool Vec3_Empty(Vector3& value);
extern float GetDistance(Vector3 value1, Vector3 value2);
extern void NormalizeAngles(Vector3& angle);
extern Vector2 CalcAngle(Vector3 local_cam, Vector3 to_point);
extern void NormalizeAngles(Vector2& angle);