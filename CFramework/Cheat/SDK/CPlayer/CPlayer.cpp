#include "CPlayer.h"

std::vector<int> PlayerBoneList = { CGameBoneID::Pelvis, CGameBoneID::LeftThigh, CGameBoneID::LeftFoot, CGameBoneID::RightThigh,
									CGameBoneID::RightFoot, CGameBoneID::LeftForearm, CGameBoneID::LeftPalm, CGameBoneID::RightForearm, CGameBoneID::RightPalm };

bool CPlayer::GetAddress(uintptr_t& ptr)
{
	address = ptr;
	return address == NULL ? false : true;
}

bool CPlayer::Update()
{
	// BoneMatrix
	uintptr_t boneMatrix = m.ReadChain(address, { offset::PlayerBody, 0x28, 0x28, 0x10 });
	bPointerList = m.Read<AllBonePointer>(boneMatrix + 0x20);

	// Position
	m_vecLocation = GetBonePosition(Base);

	if (Vec3_Empty(m_vecLocation))
		return false;

	return true;
}

void CPlayer::UpdateStatic()
{
	// some pointers
	uintptr_t m_pHealthController = m.Read<uintptr_t>(address + offset::HealthController);	// HealthController
	uintptr_t m_pHealthBody = m.Read<uintptr_t>(m_pHealthController + 0x68);
	m_pBodyController = m.Read<uintptr_t>(m_pHealthBody + 0x18);

	m_pProfile = m.Read<uintptr_t>(address + offset::Profile);
	m_pInfo = m.Read<uintptr_t>(m_pProfile + 0x28);
	m_pSetting = m.Read<uintptr_t>(m_pInfo + 0x50);
}

int CPlayer::GetSpawnType()
{
	return m.Read<int>(m_pSetting + 0x10);
}

void CPlayer::UpdateHealth()
{
	for (int j = 0; j < 7; j++)
	{
		uintptr_t body_part = m.Read<uintptr_t>(m_pBodyController + 0x30 + (j * 0x18));
		uintptr_t health_container = m.Read<uintptr_t>(body_part + 0x10);
		m_fHealth += m.Read<float>(health_container + 0x10);
		m_fHealthMax += m.Read<float>(health_container + 0x14);
	}
}

void CPlayer::UpdateBone()
{
	m_vecHeadLocation = GetBonePosition(Head);
	m_vecNeckLocation = GetBonePosition(Neck);

	if (g.g_ESP_Skeleton)
	{
		for (auto& id : PlayerBoneList)
		{
			Vector3 bone = GetBonePosition(id);

			if (Vec3_Empty(bone))
				break;

			m_pVecBoneList.push_back(bone);
		}
	}
}

uintptr_t  CPlayer::GetWeaponAnimation()
{
	return m.Read<uintptr_t>(address + offset::WeaponAnimation);
}

bool  CPlayer::IsAiming()
{
	return m.Read<bool>(GetWeaponAnimation() + 0x1bd);
}

Vector3 CPlayer::GetBonePosition(const int BoneId)
{
	uintptr_t TransformPtr = m.Read<uintptr_t>(bPointerList.address[BoneId] + 0x10);

	return GetTransformPosition(TransformPtr);
}