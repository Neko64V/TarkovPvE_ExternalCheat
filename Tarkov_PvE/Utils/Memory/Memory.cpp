#include "Memory.h"
#include <psapi.h>
#include "../Config/Config.h"

bool Memory::Init()
{
    TargetHwnd = FindWindowA(NULL, TargetWindow);

    if (!TargetHwnd)
    {
        MessageBoxA(nullptr, "Waiting tarkov...", "", MB_OK | MB_TOPMOST);

        while (!TargetHwnd)
        {
            TargetHwnd = FindWindowA(NULL, TargetWindow);

            Sleep(500);
        }
    }

    GetWindowThreadProcessId(TargetHwnd, &PID);
    pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

    if (!pHandle)
    {
        MessageBoxA(nullptr, "Failed to get pHandle", "ERROR", MB_OK | MB_TOPMOST | MB_ICONERROR);

        return false;
    }

    BaseAddress = GetModuleBase("UnityPlayer.dll");

    return true;
}

uintptr_t Memory::GetModuleBase(const std::string module)
{
    MODULEENTRY32 entry{};
    entry.dwSize = sizeof(MODULEENTRY32);

    uintptr_t result = 0;
    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);

    while (Module32Next(snapShot, &entry))
    {
        if (!module.compare(entry.szModule))
        {
            result = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
            break;
        }
    }

    if (snapShot)
        CloseHandle(snapShot);

    return result;
}

Memory m;