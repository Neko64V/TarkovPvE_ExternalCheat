#include "CItem.h"

// Example
struct C_EFT_ItemData
{
	int item_price;
	std::string item_name;
	std::string item_id;
};

// std::map
std::vector<C_EFT_ItemData>C_LootList = 
{
	{
		2500000,
		"Red Rebel",
		"5c0126f40db834002a125382"
	},
	{
		20000000,
		"Labs keycard (Red)",
		"5c1d0efb86f7744baf2e7b7b"
	},
	{
		20000000,
		" Labs keycard (Blue)",
		"1d0c5f86f7744bb2683cf0"
	},
	{
		20000000,
		"Labs keycard (Green)",
		"1d0dc586f7744baf2e7b79"
	},
	{
		20000000,
		"Labs keycard (Vioret)",
		"1e495a86f7743109743dfb"
	},
	{
		20000000,
		"Labs keycard (Black)",
		"1d0f4986f7744bb01837fa"
	},
	{
		20000000,
		"Labs keycard (Yellow)",
		"1d0d6d86f7744bb2683e1f"
	},
	{
		65000,
		"0.2 BTC",
		"faff1d86f7746c51718c9c"
	}
};

bool CItem::GetItem(uintptr_t& address)
{
	ptr = address;
	return ptr == NULL ? false : true;
}

bool CItem::Update()
{
	// Position
	uintptr_t TransformInternal = m.ReadChain(ptr, { 0x10, 0x30, 0x30, 0x8, 0x28, 0x10 });
	m_pVecLocation = GetTransformPosition(TransformInternal);

	if (Vec3_Empty(m_pVecLocation))// || m_pVecLocation.y  < -30.f)
		return false;
	
	// ClassName
	uintptr_t nameAddr = m.ReadChain(ptr, { 0x0, 0x0, 0x48 });
	std::string className = m.ReadString(nameAddr, 64);
	
	// コンテナ/死体ではなかったら
	if (className.compare("ObservedCorpse") && className.compare("Corpse"))
	{
		// ID
		char ItemID[64]{};
		uintptr_t BsgId = m.ReadChain(ptr, { 0x10, 0x28, 0xB0, 0x40, 0x50 });

		// 要改善
		const int length = m.Read<int>(BsgId + 0x10);

		for (int j = 0; j < length; j++)
			ItemID[j] = m.Read<char>(BsgId + 0x18 + (j * 0x2));

		// FUCKING SIMPLE EXAMPLE
		std::string temp = ItemID;

		if (!temp.empty())
		{
			for (auto& list_index : C_LootList)
			{
				if (list_index.item_id.find(temp) != std::string::npos)
				{
					m_iPrice = list_index.item_price;
					m_iName = list_index.item_name;
				}
			}
		}
	}

	return true;
}