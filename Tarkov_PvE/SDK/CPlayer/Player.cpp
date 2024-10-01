#include "Player.h"

std::vector<int> BasicBoneList = { Base, Head, Neck };
std::vector<int> SkeletonBoneList = { Base, Head, Neck, Pelvis, LeftThigh, LeftFoot, RightThigh, RightFoot, LeftForearm, LeftPalm, RightForearm, RightPalm };

void CPlayer::UpdateBones()
{
	BonePositionList.clear();

	for (auto& id : SkeletonBoneList)
	{
		Vector2 bScreen{};
		Vector3 bone = GetBonePositionByID(id);

		if (Vec3_Empty(bone))
			break;
		else if (!EFT.WorldToScreen(bone, bScreen))
			break;

		BonePositionList.push_back(bone);
	}
}

bool CPlayer::Update()
{
	// GetBasePointer
	Profile = m.Read<uintptr_t>(ptr + Offsets::Player::Profile);
	Info = m.Read<uintptr_t>(Profile + 0x28);
	Setting = m.Read<uintptr_t>(Info + 0x50);
	MovementContext = m.Read<uintptr_t>(ptr + 0x40);
	WeaponAnimation = m.Read<uintptr_t>(ptr + 0x1C0);

	UpdateHealth();
	UpdateBones();

	m_pSide = m.Read<int>(Info + 0x70);
	m_pSpawnType = m.Read<int>(Setting + 0x10);
	Name = GetName();

	uintptr_t temp1 = m.Read<uintptr_t>(WeaponAnimation + 0x18);
	uintptr_t temp2 = m.Read<uintptr_t>(temp1 + 0x80);
	uintptr_t fireport_transform = m.Read<uintptr_t>(temp2 + 0x10);
	Fireport = GetTransformPosition(fireport_transform);

	// Bone
	boneMatrixPtr = m.ReadChain(ptr, { 0xA8, 0x28, 0x28, 0x10 });

	if (boneMatrixPtr) {
		BasePos = GetBonePositionByID(Base);
		HeadPos = GetBonePositionByID(Head);
		NeckPos = GetBonePositionByID(Neck);

		if (Vec3_Empty(BasePos) || Vec3_Empty(HeadPos) || Vec3_Empty(NeckPos))
			return false;
	}
	else {
		return false;
	}

	return true;
}

bool CPlayer::GetPlayer(uintptr_t& address)
{
	ptr = address;
	return ptr == NULL ? false : true;
}

bool CPlayer::IsBot()
{
	return m.Read<int>(Info + 0x74) == 0 && m_pSide == SCAV;
}

bool CPlayer::IsAiming()
{
	return m.Read<bool>(GetWeaponAnimation() + 0x1bd);
}

uintptr_t CPlayer::GetWeaponAnimation()
{
	return m.Read<uintptr_t>(ptr + 0x1c0);
}

Vector3 CPlayer::GetBonePositionByID(int ID)
{
	uintptr_t BasicBoneBase = m.Read<uintptr_t>(boneMatrixPtr + 0x20 + ID * 0x8);
	uintptr_t BasicTransformPtr = m.Read<uintptr_t>(BasicBoneBase + 0x10);

	return GetTransformPosition(BasicTransformPtr);
}

std::string CPlayer::GetName()
{
	std::string temp = CheckSpawnType();

	if (!temp.compare("InValid")) {
		char pName[64]{};
		uintptr_t name_ptr = m.Read<uintptr_t>(Info + 0x10);
		int nLength = m.Read<int>(name_ptr + 0x10);
		Name.reserve(nLength);

		for (int n = 0; n < nLength; n++)
			pName[n] = m.Read<char>(name_ptr + 0x14 + (n * 0x2));

		return pName;
	}

	return temp;
}

void CPlayer::UpdateHealth()
{
	// Health
	uint64_t m_pHealthController = m.Read<uint64_t>(ptr + Offsets::Player::HealthController);
	uint64_t m_pHealthBody = m.Read<uint64_t>(m_pHealthController + 0x68);
	uint64_t m_pBodyController = m.Read<uint64_t>(m_pHealthBody + 0x18);

	m_pHealth = 0.f;
	m_pHealthMax = 0.f;

	for (int j = 0; j < 7; j++)
	{
		uintptr_t body_part = m.Read<uintptr_t>(m_pBodyController + 0x30 + (j * 0x18));
		uintptr_t health_container = m.Read<uintptr_t>(body_part + 0x10);
		float hp, max;
		hp = m.Read<float>(health_container + 0x10);
		max = m.Read<float>(health_container + 0x14);
		m_pHealth += hp;
		m_pHealthMax += max;
	}
}

std::string CPlayer::CheckSpawnType()
{
	switch (m_pSpawnType)
	{
	case Scav:
		return "Scav";
	case Scav_Sniper:
		return "Sniper";
	case Boss_Reshala:
		return "Reshala";
	case Follower_Reshala:
		return "follower";
	case Boss_Killa:
		return "Killa";
	case Boss_Shturman:
		return "Shturman";
	case Follower_Shturman:
		return "follower";
	case Boss_Glukhar:
		return "Glukhar";
	case Follower_Glukhar01:
		return "follower";
	case Follower_Glukhar02:
		return "follower";
	case Follower_Glukhar03:
		return "follower";
	case Boss_Sanitar:
		return "Sanitar";
	case Follower_Sanitar:
		return "follower";
	case Cultist01:
		return "Cultist";
	case Cultist02:
		return "Cultist";
	case Boss_Tagilla:
		return "Tagilla";
	case Scav_Rogue:
		return "Rogue";
	case Scav_Zryachiy:
		return "Zryachiy";
	case Follower_Zryachiy:
		return "follower";
	case Boss_Kollontay:
		return "Kollontay";
	case Follower_Kollontay01:
		return "follower";
	case Follower_Kollontay02:
		return "follower";
	case Boss_Kaban:
		return "Kaban";
	case Follower_Kaban:
		return "follower";
	case Scav_Raider:
		return "Raider";
	case Scav_Assault:
		return "Scav";
	case Scav_Knight:
		return "Knight";
	case Scav_BigPipe:
		return "Big Pipe";
	case Scav_Brideye:
		return "Birdeye";
	case Bloodhound:
		return "Bloodhound";
	case PMC_BEAR_PvE:
		return "BEAR";
	case PMC_USEC_PvE:
		return "USEC";
	default:
		return "unknown";
	};
}

Vector3 CPlayer::GetTransformPosition(uintptr_t& transform)
{
	__m128 result{};

	const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
	const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
	const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

	TransformAccessReadOnly pTransformAccessReadOnly = m.Read<TransformAccessReadOnly>(transform + 0x38);
	unsigned int Index = m.Read<unsigned int>(transform + 0x40);
	TransformData transform_data = m.Read<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);

	if (transform_data.pTransformArray && transform_data.pTransformIndices)
	{
		result = m.Read<__m128>(transform_data.pTransformArray + 0x30 * Index);
		int transform_index = m.Read<int>(transform_data.pTransformIndices + 0x4 * Index);
		int safe = 0;
		while (transform_index >= 0 && safe++ < 200)
		{
			Matrix34 matrix = m.Read<Matrix34>(transform_data.pTransformArray + 0x30 * transform_index);

			__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x00));	// xxxx
			__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x55));	// yyyy
			__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x8E));	// zwxy
			__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xDB));	// wzyw
			__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xAA));	// zzzz
			__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x71));	// yxwy
			__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix.vec2), result);

			result = _mm_add_ps(_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
							_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
							_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
							_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
					tmp7)), *(__m128*)(&matrix.vec0));

			transform_index = m.Read<int>(transform_data.pTransformIndices + 0x4 * transform_index);
		}
	}

	return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}