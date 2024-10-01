#pragma once
#include "../Game/SDK.h"

class CExfil
{
public:
	uintptr_t ptr;

	int m_pStatus;
	Vector3 m_pVecLocation;
	std::string ExfilName;

	bool GetEntity(uintptr_t& address);
	bool Update();
private:
	Vector3 GetTransformPosition(uintptr_t& transform);
};