#pragma once
#include "Struct.h"

namespace offset
{
	// GameObjectManager
	constexpr auto Tarkov_Camera = 0x179F500;
	constexpr auto GameObjectManager = 0x17FFD28;
	
	// EFT.GameWorld
	constexpr auto ExfilController	 = 0x18;
	constexpr auto LootList			 = 0xC8;
	constexpr auto RegisteredPlayers = 0xF0;
	constexpr auto GrenadeList		 = 0x1A0;

	// EFT.Player
	constexpr auto PlayerBody = 0xA8;
	constexpr auto WeaponAnimation  = 0x1C0;
	constexpr auto Profile			= 0x5C0;
	constexpr auto Physics			= 0x5D0;
	constexpr auto HealthController = 0x600;
};

class EFT
{
private:
	GameObjectManager m_GOM{};
	uintptr_t m_GameWorld;
	uintptr_t m_fpsCamera;
public:
	Matrix	  m_ViewMatrix{};
	uintptr_t m_localGameWorld;

	bool InitAddress();
	bool Update();
	bool UpdateCamera();
	Matrix GetViewMatrix();
	uintptr_t GetLocalGameWorld();
};

extern bool Vec3_Empty(const Vector3& value);
extern float GetDistance(Vector3 value1, Vector3 value2);
extern Vector3 GetTransformPosition(uintptr_t& transform);
extern bool WorldToScreen(Matrix ViewMatrix, const Vector2& GameSize, Vector3& vIn, Vector2& vOut);
extern uintptr_t GetObjectFromList(uintptr_t listPtr, uintptr_t lastObjectPtr, const char* objectName);
