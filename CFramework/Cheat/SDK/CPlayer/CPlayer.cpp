#include "CPlayer.h"

std::vector<int> PlayerBoneList = { CGameBoneID::Base, CGameBoneID::Head, CGameBoneID::Neck, CGameBoneID::Pelvis, CGameBoneID::LeftThigh, CGameBoneID::LeftFoot, CGameBoneID::RightThigh,
									CGameBoneID::RightFoot, CGameBoneID::LeftForearm, CGameBoneID::LeftPalm, CGameBoneID::RightForearm, CGameBoneID::RightPalm };

bool CPlayer::GetEntity(uintptr_t& address)
{
	ptr = address;
	return ptr == NULL ? false : true;
}

bool CPlayer::Update()
{
	// BoneMatrix
	m_pBoneMatrix = m.ReadChain(ptr, { 0xA8, 0x28, 0x28, 0x10 });

	// Position
	m_pVecLocation = GetBonePosition(Base);

	// ESP•\Ž¦‚³‚ê‚È‚­‚È‚é ToDo
	if (Vec3_Empty(m_pVecLocation))
		return false;

	UpdateHealth(); // HP
	UpdateBone();	// Bone

	// Pointer
	uintptr_t Profile = m.Read<uintptr_t>(ptr + offset::Profile);
	uintptr_t Info = m.Read<uintptr_t>(Profile + 0x28);
	uintptr_t Setting = m.Read<uintptr_t>(Info + 0x50);

	m_pSpawnType = m.Read<int>(Setting + 0x10);

	return true;
}

void CPlayer::UpdateHealth()
{
	uintptr_t m_pHealthController = m.Read<uintptr_t>(ptr + offset::HealthController);	// HealthController
	uintptr_t m_pHealthBody = m.Read<uintptr_t>(m_pHealthController + 0x68);
	uintptr_t m_pBodyController = m.Read<uintptr_t>(m_pHealthBody + 0x18);

	m_pHealth = 0.f, m_pHealthMax = 0.f;

	for (int j = 0; j < 7; j++)
	{
		uintptr_t body_part = m.Read<uintptr_t>(m_pBodyController + 0x30 + (j * 0x18));
		uintptr_t health_container = m.Read<uintptr_t>(body_part + 0x10);
		m_pHealth += m.Read<float>(health_container + 0x10);
		m_pHealthMax += m.Read<float>(health_container + 0x14);
	}
}

void CPlayer::UpdateBone()
{
	for (auto& id : PlayerBoneList)
	{
		Vector3 bone = GetBonePosition(id);

		if (Vec3_Empty(bone))
			break;

		m_pBoneList.push_back(bone);
	}
}

uintptr_t  CPlayer::GetWeaponAnimation()
{
	return m.Read<uintptr_t>(ptr + offset::WeaponAnimation);
}

bool  CPlayer::IsAiming()
{
	return m.Read<bool>(GetWeaponAnimation() + 0x1bd);
}

Vector3 CPlayer::GetBonePosition(int BoneId)
{
	uintptr_t BasicBoneBase = m.Read<uintptr_t>(m_pBoneMatrix + 0x20 + (BoneId * 0x8));
	uintptr_t BasicTransformPtr = m.Read<uintptr_t>(BasicBoneBase + 0x10);

	return GetTransformPosition(BasicTransformPtr);
}