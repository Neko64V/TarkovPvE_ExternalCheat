#include "Item.h"

bool CItem::GetEntity(uintptr_t& address)
{
	ptr = address;
	return ptr == NULL ? false : true;
}

std::string CItem::GetItemClassName()
{
	char name[64]{};
	uintptr_t sptr = m.ReadChain(ptr, { 0x0, 0x0, 0x48 });
	ReadProcessMemory(m.pHandle, (void*)(sptr), name, sizeof(name), nullptr);

	return name;
}

std::string CItem::GetID()
{
	char id[128]{};
	uintptr_t BsgId = m.ReadChain(ptr, { 0x10, 0x28, 0xB0, 0x40, 0x50 });
	int length = m.Read<int>(BsgId + 0x10);

	for (int j = 0; j < length; j++)
		id[j] = m.Read<char>(BsgId + 0x18 + (j * 0x2));

	return id;
}

bool CItem::Update()
{
	uintptr_t TransformInternal = m.ReadChain(ptr, { 0x10, 0x30, 0x30, 0x8, 0x28, 0x10 });
	ItemPos = GetTransformPosition(TransformInternal);

	if (Vec3_Empty(ItemPos))
		return false;
	else if (ItemPos.y < -100.f)
		return false;

	std::string ClassName = GetItemClassName();

	ItemID = GetID();

	// Corpse
	if (ClassName == "Corpse")
		ItemName = ClassName;
	else if (ClassName == "LootableContainer")
		ItemName = "Container";
	else if (ClassName == "ObservedLootItem")	
	{
		ItemName = "item"; // ToDo
		/*
		for (const auto& item : AllItemList)
		{
			if (ItemID == item.id)
			{
				ItemName = item.name;
				ItemPrice = item.price;

				return true;
			}
		}
		*/
	}

	return true;
}

Vector3 CItem::GetTransformPosition(uintptr_t& transform)
{
	__m128 result{};

	const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
	const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
	const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

	TransformAccessReadOnly pTransformAccessReadOnly = m.Read<TransformAccessReadOnly>(transform + 0x38);
	unsigned int Index = m.Read<unsigned int>(transform + 0x40);
	TransformData transform_data = m.Read<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);

	if (transform_data.pTransformArray && transform_data.pTransformIndices)
	{
		result = m.Read<__m128>(transform_data.pTransformArray + 0x30 * Index);
		int transform_index = m.Read<int>(transform_data.pTransformIndices + 0x4 * Index);
		int safe = 0;
		while (transform_index >= 0 && safe++ < 200)
		{
			Matrix34 matrix = m.Read<Matrix34>(transform_data.pTransformArray + 0x30 * transform_index);

			__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x00));	// xxxx
			__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x55));	// yyyy
			__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x8E));	// zwxy
			__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xDB));	// wzyw
			__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xAA));	// zzzz
			__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x71));	// yxwy
			__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix.vec2), result);

			result = _mm_add_ps(_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
							_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
							_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
							_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
					tmp7)), *(__m128*)(&matrix.vec0));

			transform_index = m.Read<int>(transform_data.pTransformIndices + 0x4 * transform_index);
		}
	}

	return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}