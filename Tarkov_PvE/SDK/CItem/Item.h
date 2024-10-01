#pragma once
#include "../Game/SDK.h"

class CItem
{
public:
	uintptr_t ptr;
	Vector3 ItemPos;

	std::string ItemID;
	std::string ItemName;
	int ItemPrice;

	bool GetEntity(uintptr_t& address);
	bool Update(); 
	std::string GetItemClassName();
	std::string GetID();
private:
	Vector3 GetTransformPosition(uintptr_t& transform);
};