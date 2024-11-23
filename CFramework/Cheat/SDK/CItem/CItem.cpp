#include "CItem.h"

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

	if (Vec3_Empty(m_pVecLocation) || -35.f > m_pVecLocation.z)
		return false;

	// ClassName
	//char* iName[64]{};
	//uintptr_t nameAddr = m.ReadChain(ptr, { 0x10, 0x30, 0x60 });
	//m.ReadString(nameAddr, iName, sizeof(iName));

	// ID
	//char id[32]{};
	//uintptr_t BsgId = m.ReadChain(ptr, { 0x10, 0x28, 0xB0, 0x40, 0x50 });
	//int length = m.Read<int>(BsgId + 0x10);

	//for (int j = 0; j < length; j++)
	//	id[j] = m.Read<char>(BsgId + 0x18 + (j * 0x2));

	return true;
}