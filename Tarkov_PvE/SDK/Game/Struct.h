#pragma once
#include "../../Utils/SimpleMath/SimpleMath.h"
using namespace DirectX::SimpleMath;

struct Matrix34
{
	Vector4 vec0;
	Vector4 vec1;
	Vector4 vec2;
};

struct UnityList  // System.Collections.Generic.List
{
	uintptr_t Base = 0x10;
	int Count = 0x18;
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

struct cameras
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
	USEC = 1,
	BEAR = 2,
	SCAV = 4
};

enum BoneIdList : int
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