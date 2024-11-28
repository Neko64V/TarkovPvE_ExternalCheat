#include "GameSDK.h"

bool Vec3_Empty(const Vector3& value)
{
    return value == Vector3();
}

float GetDistance(Vector3 value1, Vector3 value2)
{
    float num = value1.x - value2.x;
    float num2 = value1.y - value2.y;
    float num3 = value1.z - value2.z;

    return sqrt(num * num + num2 * num2 + num3 * num3);
}

Vector3 GetTransformPosition(uintptr_t& transform)
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

bool WorldToScreen(Matrix ViewMatrix, const Vector2& GameSize, Vector3& vIn, Vector2& vOut)
{
    Matrix m = ViewMatrix.Transpose();
    Vector3 translationVector = Vector3(m._41, m._42, m._43);
    Vector3 up = Vector3(m._21, m._22, m._23);
    Vector3 right = Vector3(m._11, m._12, m._13);

    float w = translationVector.Dot(vIn) + m._44;

    if (w < 0.098f)
        return false;

    float y = up.Dot(vIn) + m._24;
    float x = right.Dot(vIn) + m._14;
    vOut = Vector2((GameSize.x / 2) * (1.f + x / w), (GameSize.y / 2) * (1.f - y / w));

    return true;
}

uintptr_t GetObjectFromList(uintptr_t listPtr, uintptr_t lastObjectPtr, const char* objectName)
{
	char ObjectName[128]{};
	uintptr_t classNamePtr = 0;
	BaseObject activeObject = m.Read<BaseObject>(listPtr);
	BaseObject lastObject = m.Read<BaseObject>(lastObjectPtr);

	if (activeObject.object != NULL)
	{
		while (activeObject.object != 0 && activeObject.object != lastObject.object)
		{
			classNamePtr = m.Read<uint64_t>(activeObject.object + 0x60);
			m.ReadString(classNamePtr, ObjectName, sizeof(ObjectName));

			if (strcmp(ObjectName, objectName) == 0)
				return activeObject.object;

			activeObject = m.Read<BaseObject>(activeObject.nextObjectLink);
		}
	}
	if (lastObject.object != NULL)
	{
		classNamePtr = m.Read<uint64_t>(lastObject.object + 0x60);
		m.ReadString(classNamePtr, ObjectName, sizeof(ObjectName));

		if (strcmp(ObjectName, objectName) == 0)
			return lastObject.object;
	}

	ZeroMemory(ObjectName, sizeof(ObjectName));
	Sleep(1);

	return 0;
}

bool EFT::InitAddress()
{
	// GameObjectManager
	uintptr_t GomPtr = m.Read<uintptr_t>(m.m_gBaseAddress + offset::GameObjectManager);
	m_GOM = m.Read<GameObjectManager>(GomPtr);

	// GameWorld
	uintptr_t activeNodes = m.Read<uintptr_t>(m_GOM.ActiveNodes);
	uintptr_t lastActiveNode = m.Read<uintptr_t>(m_GOM.LastActiveNode);
	m_gameWorld = GetObjectFromList(activeNodes, lastActiveNode, "GameWorld");

	return m_gameWorld == 0 ? false : true;
}

bool EFT::Update()
{
	// LocalGameWorld
	m_localGameWorld = m.ReadChain(m_gameWorld, { 0x30, 0x18, 0x28 });

	if (m_localGameWorld == 0) {
		if (!InitAddress())
			return false;
	}

	return true;
}

bool EFT::UpdateCamera()
{
	Camera all_cameras = m.Read<Camera>(m.Read<uintptr_t>(m.m_gBaseAddress + offset::Tarkov_Camera));

	if (all_cameras.count == 0)
		return false;

	for (int i = 0; i < all_cameras.count; i++)
	{
		uintptr_t camera = m.Read<uintptr_t>(all_cameras.list + (i * 0x8));
		if (camera != NULL)
		{
			uintptr_t camera_obj = m.Read<uintptr_t>(camera + 0x30);
			uintptr_t camera_name_ptr = m.Read<uintptr_t>(camera_obj + 0x60);

			if (!camera_name_ptr)
				return false;

			char name[64]{};
			m.ReadString(camera_name_ptr, name, sizeof(name));

			if (strcmp(name, "FPS Camera") == 0) {
				m_fpsCamera = camera_obj;
				return true;
			}
		}
	}

	return false;
}

Matrix EFT::GetViewMatrix()
{
	uintptr_t dw = m.Read<uintptr_t>(m_fpsCamera + 0x30);
	uintptr_t viewmatrix_ptr = m.Read<uintptr_t>(dw + 0x18);

	return m.Read<Matrix>(viewmatrix_ptr + 0xDC);
}

uintptr_t EFT::GetLocalGameWorld()
{
	return this->m_localGameWorld;
}