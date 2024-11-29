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

    // Functions
    bool GetEntity(uintptr_t& address);
	bool Update();
	void UpdateBone();
	void UpdateHealth();
	bool IsAiming();
	uintptr_t GetWeaponAnimation();
	Vector3 GetBonePosition(int BoneId);
};