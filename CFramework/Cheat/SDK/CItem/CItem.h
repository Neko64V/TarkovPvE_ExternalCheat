#pragma once
#include "../GameSDK/GameSDK.h"

class CItem
{
private:

public:
    uintptr_t ptr;

	int m_iPrice;
	std::string m_iName;
	Vector3 m_pVecLocation;

	bool GetItem(uintptr_t& address);
	bool Update();
};