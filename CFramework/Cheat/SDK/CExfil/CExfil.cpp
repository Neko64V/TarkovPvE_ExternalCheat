#include "CExfil.h"

bool CExfil::GetExfil(uintptr_t& address)
{
	ptr = address;
	return ptr == NULL ? false : true;
}

bool CExfil::Update()
{
	// Status
	m_pExfilStatus = m.Read<int>(ptr + 0xA8); 

	if (m_pExfilStatus == CLOSE)
		return false;

	// Position
	uintptr_t TransformInternal = m.ReadChain(ptr, { 0x10, 0x30, 0x30, 0x8, 0x28, 0x10 });
	m_pVecLocation = GetTransformPosition(TransformInternal);

	if (Vec3_Empty(m_pVecLocation))
		return false;

	// Name
	uintptr_t eSetting = m.Read<uintptr_t>(ptr + 0x58);
	uintptr_t NamePtr = m.Read<uintptr_t>(eSetting + 0x10);

	if (!NamePtr)
		return false;

	char pName[64]{};
	int length = m.Read<int>(NamePtr + 0x10);

	for (int i = 0; i < length; i++)
		pName[i] = m.Read<char>(NamePtr + 0x14 + (i * 0x2));

	m_pExfilName = pName;

	return true;
}