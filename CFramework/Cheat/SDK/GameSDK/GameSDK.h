#pragma once
#include "Struct.h"

namespace offset
{
	// GameObjectManager
	constexpr auto Tarkov_Camera = 0x179F500;
	constexpr auto GameObjectManager = 0x17FFD28;
	
	// EFT.GameWorld
	constexpr auto ExfilController	 = 0x20;
	constexpr auto LootList			 = 0x100;
	constexpr auto RegisteredPlayers = 0x128;
	constexpr auto MainPlayer		 = 0x190;
	constexpr auto GrenadeList		 = 0x1E8;

	// EFT.Player
	constexpr auto PlayerBody		= 0xB8;
	constexpr auto WeaponAnimation  = 0x1D8;
	constexpr auto Profile			= 0x648;
	constexpr auto Physics			= 0x658;
	constexpr auto HealthController = 0x688;

	// WeaponAnimation
	//constexpr auto IsAiming			= 0x0000;
};

class EFT
{
private:
	GameObjectManager m_GOM{};
	uintptr_t m_gameWorld;
	uintptr_t m_fpsCamera;
public:
	Matrix	  m_ViewMatrix;
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