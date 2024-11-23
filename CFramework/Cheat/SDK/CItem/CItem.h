#pragma once
#include "../GameSDK/GameSDK.h"

class CItem
{
private:

public:
    uintptr_t ptr;

	Vector3 m_pVecLocation;
	std::string m_ItemName;

	bool GetItem(uintptr_t& address);
	bool Update();
};