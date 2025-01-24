#include "Memory.h"

bool Memory::AttachProcess(const char* targetName, int InitMode)
{
    if (InitMode == WINDOW_TITLE || InitMode == WINDOW_CLASS)
    {
        HWND TargetHwnd = InitMode == WINDOW_TITLE ? FindWindowA(NULL, targetName) : FindWindowA(targetName, NULL);

        if (!TargetHwnd) {
            MessageBoxA(nullptr, "Please open ##GAME_NAME", "Init Fail", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }

        GetWindowThreadProcessId(TargetHwnd, &PID);
    }
    else if (InitMode == PROCESS)
    {
        PROCESSENTRY32 process = GetProcess(targetName);

        if (process.th32ProcessID == 0) {
            MessageBoxA(nullptr, "Please open ##GAME_NAME", "Init Fail", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }

        PID = process.th32ProcessID;
    }
    else
    {
        MessageBoxA(nullptr, "Invalid memory option", "Init Fail", MB_TOPMOST | MB_ICONERROR | MB_OK);
        return false;
    }

    // プロセスハンドルを取得
    pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

    if (!pHandle) {
        MessageBoxA(nullptr, "Failed to get process handle", "Init Error", MB_TOPMOST | MB_ICONERROR | MB_OK); // 管理者として実行するかアンチチートをバイパスする
        return false;
    }

    // ベースアドレスを取得
    m_gBaseAddress = GetModuleBase("UnityPlayer.dll");

    if (m_gBaseAddress == 0)
        MessageBoxA(nullptr, "Base is 0", "WARNING", MB_OK | MB_TOPMOST);

    return true;
}

void Memory::DetachProcess()
{
    CloseHandle(pHandle);
}

uintptr_t Memory::GetModuleBase(const std::string moduleName)
{
    MODULEENTRY32 entry{};
    entry.dwSize = sizeof(MODULEENTRY32);
    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);

    while (Module32Next(snapShot, &entry))
    {
        if (!moduleName.compare(entry.szModule))
        {
            CloseHandle(snapShot);
            return reinterpret_cast<uintptr_t>(entry.modBaseAddr);
        }
    }

    if (snapShot)
        CloseHandle(snapShot);

    return reinterpret_cast<uintptr_t>(entry.modBaseAddr);
}

PROCESSENTRY32 Memory::GetProcess(const std::string processName)
{
    PROCESSENTRY32 entry{};
    entry.dwSize = sizeof(PROCESSENTRY32);
    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    while (Process32Next(snapShot, &entry))
    {
        if (!processName.compare(entry.szExeFile))
        {
            CloseHandle(snapShot);
            return entry;
        }
    }

    CloseHandle(snapShot);

    return PROCESSENTRY32();
}

Memory m;