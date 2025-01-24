#pragma once
#include "../GameSDK/GameSDK.h"

class CExfil
{
private:

public:
    uintptr_t ptr;

	// データ
	int m_pExfilStatus;
	Vector3 m_pVecLocation;
	std::string m_pExfilName;

	bool GetAddress(uintptr_t& address);
	int GetStatus();
	std::string GetName();
	bool Update();
};