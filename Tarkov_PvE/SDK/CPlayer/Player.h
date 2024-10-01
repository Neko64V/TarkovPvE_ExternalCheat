#pragma once
#include <vector>
#include "../Game/SDK.h"

class CPlayer
{
private:
	void UpdateHealth();
	std::string GetName();
	std::string CheckSpawnType();

    Vector3 GetTransformPosition(uintptr_t& transform);
public:
	uintptr_t ptr;
	uintptr_t Profile;
	uintptr_t Info;
	uintptr_t Setting;
	uintptr_t MovementContext;
	uintptr_t WeaponAnimation;
	uintptr_t boneMatrixPtr;

	int m_pSpawnType;
	int m_pSide;
	float m_pHealth;
	float m_pHealthMax;

	Vector3 Fireport;

	Vector3 BasePos;
	Vector3 HeadPos;
	Vector3 NeckPos;
	std::string Name;

	// Bone
	void UpdateBones();
	std::vector<Vector3> BonePositionList;

	bool GetPlayer(uintptr_t& address);
	bool IsBot();
	bool IsAiming();
	bool Update();
	uintptr_t GetWeaponAnimation();
	Vector3 GetBonePositionByID(int ID);
};