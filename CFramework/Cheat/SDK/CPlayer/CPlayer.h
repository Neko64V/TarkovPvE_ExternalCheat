#pragma once
#include "../GameSDK/GameSDK.h"

struct AllBonePointer {
	uint64_t address[150]{};
};

class CPlayer
{
private:
	
public:
    uintptr_t address;

	uintptr_t m_pProfile;
	uintptr_t m_pInfo;
	uintptr_t m_pSetting;
	uintptr_t m_pBodyController;

	// データ
	float m_fHealth;
	float m_fHealthMax;
	int m_iSpawnType;
	Vector3 m_vecLocation;
	Vector3 m_vecHeadLocation;
	Vector3 m_vecNeckLocation;
	AllBonePointer bPointerList{};
	std::vector<Vector3> m_pVecBoneList;
	
    // Functions
	bool GetAddress(uintptr_t& ptr);
	bool Update();
	void UpdateStatic();
	void UpdateBone();
	void UpdateHealth();

	bool IsAiming();

	int GetSpawnType();
	uintptr_t GetWeaponAnimation();
	Vector3 GetBonePosition(const int BoneId);
};