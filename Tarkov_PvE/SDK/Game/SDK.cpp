#include "SDK.h"

bool Tarkov::InitAddress()
{
    // GameObjectManager
    uintptr_t GOM_Ptr = m.Read<uintptr_t>(m.BaseAddress + Offsets::GameObjectManager);
    this->GOM = m.Read<GameObjectManager>(GOM_Ptr);

    // GameWorld
    uintptr_t activeNodes = m.Read<uintptr_t>(GOM.ActiveNodes);
    uintptr_t lastActiveNode = m.Read<uintptr_t>(GOM.LastActiveNode);
    GameWorld = GetObjectFromList(activeNodes, lastActiveNode, "GameWorld");

    if (!this->GameWorld)
    {
        GOM = GameObjectManager();
        GameWorld = NULL;
        localGameWorld = NULL;
        return false;
    }

    // LocalGameWorld
    localGameWorld = m.ReadChain(GameWorld, { 0x30, 0x18, 0x28 });

    if (!localGameWorld)
    {
        GOM = {};
        GameWorld = NULL;
        localGameWorld = NULL;
        return false;
    }

    if (!GetCamera())
        return false;

    return true;
}

uintptr_t Tarkov::GetObjectFromList(uintptr_t listPtr, uintptr_t lastObjectPtr, const char* objectName)
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
            ReadProcessMemory(m.pHandle, (void*)(classNamePtr), &ObjectName, sizeof(ObjectName), NULL);

            if (strcmp(ObjectName, objectName) == 0)
                return activeObject.object;

            activeObject = m.Read<BaseObject>(activeObject.nextObjectLink);
        }
    }
    if (lastObject.object != NULL)
    {
        classNamePtr = m.Read<uint64_t>(lastObject.object + 0x60);

        ReadProcessMemory(m.pHandle, (void*)(classNamePtr), &ObjectName, sizeof(ObjectName), NULL);

        if (strcmp(ObjectName, objectName) == 0)
            return lastObject.object;
    }

    ZeroMemory(ObjectName, sizeof(ObjectName));

    return 0;
}

bool Tarkov::GetCamera()
{
    cameras all_cameras = m.Read<cameras>(m.Read<uintptr_t>(m.BaseAddress + 0x179F500));

    if (all_cameras.count == 0)
        return false;

    for (int i = 0; i < all_cameras.count; i++)
    {
        auto camera = m.Read<uintptr_t>(all_cameras.list + ((uint64_t)i * 0x8));
        if (camera != NULL)
        {
            auto camera_obj = m.Read<uintptr_t>(camera + 0x30);
            auto camera_name_ptr = m.Read<uintptr_t>(camera_obj + 0x60);

            if (!camera_name_ptr)
                return false;

            char name[64];
            for (int j = 0; j < sizeof(name); j++)
            {
                name[j] = m.Read<char>(camera_name_ptr + j);
                if (name[j] == NULL)
                    break;
            }

            if (strcmp(name, "FPS Camera") == 0) {
                fpsCamera = camera_obj;
                return true;
            }
        }
    }

    return false;
}

bool Tarkov::WorldToScreen(const Vector3& vIn, Vector2& vOut)
{
    uint64_t base = m.Read<uint64_t>(fpsCamera + 0x30);
    base = m.Read<uint64_t>(base + 0x18);
    ViewMatrix = m.Read<Matrix>(base + 0xDC);

    Matrix m = ViewMatrix.Transpose();
    Vector3 translationVector = Vector3(m._41, m._42, m._43);
    Vector3 up = Vector3(m._21, m._22, m._23);
    Vector3 right = Vector3(m._11, m._12, m._13);

    float w = translationVector.Dot(vIn) + m._44;

    if (w < 0.098f)
        return false;

    float y = up.Dot(vIn) + m._24;
    float x = right.Dot(vIn) + m._14;
    vOut = Vector2((g.GameSize.right / 2) * (1.f + x / w), (g.GameSize.bottom / 2) * (1.f - y / w));

    return true;
}

bool Vec3_Empty(Vector3& value)
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

void NormalizeAngles(Vector3& angle)
{
    while (angle.x > 89.0f)
        angle.x -= 180.f;

    while (angle.x < -89.0f)
        angle.x += 180.f;

    while (angle.y > 180.f)
        angle.y -= 360.f;

    while (angle.y < -180.f)
        angle.y += 360.f;
}

Vector2 CalcAngle(Vector3 source, Vector3 point)
{
    Vector2 vOut{};

    return vOut;
}

void NormalizeAngles(Vector2& angle)
{
    while (angle.x > 89.0f)
        angle.x -= 180.f;

    while (angle.x < -89.0f)
        angle.x += 180.f;

    while (angle.y > 180.f)
        angle.y -= 360.f;

    while (angle.y < -180.f)
        angle.y += 360.f;
}