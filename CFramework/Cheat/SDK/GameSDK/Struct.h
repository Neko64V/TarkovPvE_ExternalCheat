#pragma once
#include "../../../Framework/Utils/Utils.h"

struct Matrix34
{
	Vector4 vec0;
	Vector4 vec1;
	Vector4 vec2;
};

struct UnityList
{
	char trash[0x10]{};
	uintptr_t ctx   = 0x10;
	int count		= 0x18;
};

struct BaseObject
{
    uint64_t previousObjectLink;
    uint64_t nextObjectLink;
    uint64_t object;
};
struct GameObjectManager
{
    uint64_t lastTaggedObject;
    uint64_t taggedObjects;
    uint64_t lastActiveObject;
    uint64_t activeObjects;
    uint64_t LastActiveNode;
    uint64_t ActiveNodes;
};

struct TransformAccessReadOnly
{
	uintptr_t	pTransformData;
    int			index;
};

struct TransformData
{
    uintptr_t pTransformArray;
	uintptr_t pTransformIndices;
};

struct Camera
{
    uint64_t list;
    uint64_t min;
    uint64_t count;
    uint64_t max;
};

enum ExfilStatus : int
{
	CLOSE = 1,
	NOTREADY = 2,
	OPEN = 4,
	READY = 6
};

enum PlayerSide : int
{
	CUSEC = 1,
	CBEAR = 2,
	CSCAV = 4
};

enum CGameBoneID : int
{
	Base = 0,
	Pelvis = 14,
	LeftThigh = 16,
	LeftCalf = 17,
	LeftFoot = 18,
	LeftToe = 19,
	RightThigh = 21,
	RightCalf = 22,
	RightFoot = 23,
	RightToe = 24,
	Spine1 = 29,
	Spine2 = 36,
	Spine3 = 37,
	LeftCollarbone = 89,
	LeftUpperarm = 90,
	LeftForearm = 92,
	LeftPalm = 94,
	RightCollarbone = 110,
	RightUpperarm = 111,
	RightForearm = 113,
	RightPalm = 115,
	Neck = 132,
	Head = 133
};

enum SkeletonBoneID : int
{
	CPelvis = 0,
	CLeftThigh = 1,
	CLeftFoot = 2,
	CRightThigh = 3,
	CRightFoot = 4,
	CLeftForearm = 5,
	CLeftPalm = 6,
	CRightForearm = 7,
	CRightPalm = 8
};

// SPT
enum WildSpanwType : int
{
	// Scav
	SNIPER_SCAV = 0,
	SCAV = 1,

	// Customs
	RESHALA_BOSS = 3,
	RESHALA_FOLLOW = 5,

	// InterChange
	KILLA_BOSS = 6,

	// Woods
	SHTURMAN_BOSS = 7,
	SHTURMAN_FOLLOW = 8,

	// Global
	RAIDER_SCAV = 9,

	// Reserve
	GLUKHAR_BOSS = 11,
	GLUKHAR_FOLLOW_01 = 12,
	GLUKHAR_FOLLOW_02 = 13,
	GLUKHAR_FOLLOW_03 = 14,

	// Shoreline
	SANITOR_FOLLOW = 16,
	SANITOR_BOSS = 17,

	// Global
	NORMAL_SCAV = 19, // è≠Çµã≠âªÇ≥ÇÍÇΩScav (?????
	CULTIST_01_SCAV = 20,
	CULTIST_02_SCAV = 21,

	// Factory
	TAGILLA_BOSS = 22,

	// Lighthouse
	ROGUE_SCAV = 24,

	// Global
	KNIGHT_SCAV = 26,
	BIGPIPE_SCAV = 27,
	BRIDEYE_SCAV = 28,

	// Lighthouse
	ZRYACHIY_BOSS = 29,
	ZRYACHIY_FOLLOW = 30,

	// Street of Tarkov
	KABAN_BOSS = 32,

	// Global (?
	BLOODHOUND_SCAV = 35,

	KABAN_FOLLOW = 36,
	KOLLONTAY_BOSS = 43,
	KOLLONTAY_FOLLOW_01 = 44,
	KOLLONTAY_FOLLOW_02 = 45,

	// Global
	BTR_VEHICLE = 46,

	// Player
	PMC_BEAR_PvE = 49,
	PMC_USEC_PvE = 50,

	// Global (?
	ASSAULT_SCAV_RU = 51,		// RU - Scav
	ASSAULT_SCAV_UN = 52	// US - Scav
};