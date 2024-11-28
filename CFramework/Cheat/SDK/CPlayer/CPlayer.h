#pragma once
#include "../GameSDK/GameSDK.h"

class CPlayer
{
private:
	
public:
    uintptr_t ptr;

	uintptr_t m_pBoneMatrix;

	// データ
	float m_pHealth;
	float m_pHealthMax;
	int m_pSpawnType;
	Vector3 m_pVecLocation;
	Vector3 m_pHeadLocation;
	Vector3 m_pNeckLocation;
	std::vector<Vector3> m_pBoneList;

	void UpdateHealth();
	void UpdateBone();

    // Functions
    bool GetEntity(uintptr_t& address);
	bool Update();
	uintptr_t GetWeaponAnimation();
	bool IsAiming();
	Vector3 GetBonePosition(int BoneId);
};