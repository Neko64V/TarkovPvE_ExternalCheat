#include "Overlay.h"

LONG MenuStyle = WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
LONG ESPStyle = WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

bool Overlay::InitOverlay(const std::string targetName, int mode)
{
    switch (mode)
    {
    case InitMode::WINDOW_TITLE:
        TargetHwnd = FindWindowA(NULL, targetName.c_str());

        if (!TargetHwnd) {
            MessageBoxA(nullptr, "Target not found Mode: 0", "Init Error", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }
        break;
    case InitMode::WINDOW_CLASS:
        TargetHwnd = FindWindowA(targetName.c_str(), NULL);

        if (!TargetHwnd) {
            MessageBoxA(nullptr, "Target not found Mode: 1", "Init Error", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }
        break;
    case InitMode::PROCESS: 
        TargetHwnd = GetTargetWindow(targetName);

        if (!TargetHwnd) {
            MessageBoxA(nullptr, "Target not found Mode: 2", "Init Error", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }
        break;
    default:
        return false;
    }

    GetClassNameA(TargetHwnd, TargetClass, sizeof(TargetClass));

    return CreateOverlay();
}

// [+]オーバーレイのウィンドウをゲームのウィンドウの上やサイズに合わせるための処理です。
void Overlay::OverlayManager()
{
    // Window Check
    HWND CheckHwnd = FindWindowA(TargetClass, NULL);
    if (!CheckHwnd) {
        g.process_active = false;
        return;
    }

    // StreamProof
    DWORD Flag = NULL;
    GetWindowDisplayAffinity(Hwnd, &Flag);
    if (g.g_StreamProof && Flag == WDA_NONE)
        SetWindowDisplayAffinity(Hwnd, WDA_EXCLUDEFROMCAPTURE);
    else if (!g.g_StreamProof && Flag == WDA_EXCLUDEFROMCAPTURE)
        SetWindowDisplayAffinity(Hwnd, WDA_NONE);

    // Window Style Changer
    HWND ForegroundWindow = GetForegroundWindow();
    LONG TmpLong = GetWindowLong(Hwnd, GWL_EXSTYLE);

    if (g.ShowMenu && MenuStyle != TmpLong)
        SetWindowLong(Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
    else if (!g.ShowMenu && ESPStyle != TmpLong)
        SetWindowLong(Hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);

    // MenuToggle
    if (GetKeyState(g.g_MenuKey) && !g.ShowMenu) {
        g.ShowMenu = !g.ShowMenu;

        if (ForegroundWindow != Hwnd)
            SetForegroundWindow(Hwnd);
    }
    else if (!GetKeyState(g.g_MenuKey) && g.ShowMenu) {
        g.ShowMenu = !g.ShowMenu;

        if (ForegroundWindow != CheckHwnd)
            SetForegroundWindow(CheckHwnd);
    }

    // Window resizer
    RECT TmpRect{};
    POINT TmpPoint{};
    GetClientRect(CheckHwnd, &TmpRect);
    ClientToScreen(CheckHwnd, &TmpPoint);

    // Resizer
    if (TmpRect.left != g.GameRect.left || TmpRect.bottom != g.GameRect.bottom || TmpRect.top != g.GameRect.top || TmpRect.right != g.GameRect.right || TmpPoint.x != g.GamePoint.x || TmpPoint.y != g.GamePoint.y)
    {
        g.GameRect = TmpRect;
        g.GamePoint = TmpPoint;
        SetWindowPos(Hwnd, nullptr, TmpPoint.x, TmpPoint.y, g.GameRect.right, g.GameRect.bottom, SWP_NOREDRAW);
    }
}

HWND Overlay::GetTargetWindow(const std::string processName)
{
    DWORD PID = NULL;
    PROCESSENTRY32 entry{};
    entry.dwSize = sizeof(PROCESSENTRY32);
    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    while (Process32Next(snapShot, &entry))
    {
        if (!processName.compare(entry.szExeFile))
        {
            PID = entry.th32ProcessID;
            break;
        }
    }

    CloseHandle(snapShot);

    HWND hwnd = GetTopWindow(NULL);
    do {
        if (GetWindowLong(hwnd, 8) != 0 || !IsWindowVisible(hwnd))
            continue;
        DWORD ProcessID;
        GetWindowThreadProcessId(hwnd, &ProcessID);
        if (PID == ProcessID)
            return hwnd;
    } while ((hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) != NULL);

    return NULL;
}