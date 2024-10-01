#include "Overlay\Overlay.h"
#include "Cheat\Cheat.h"

Cheat Ct = Cheat();
Tarkov EFT = Tarkov();
Overlay Ov = Overlay();

int main()
{
    if (!m.Init())
        return 1;

    if (!Ov.CreateOverlay())
        return 2;

    g.Run = true;
    Ct.Init();
    std::thread([&]() { Ct.UpdateList(); }).detach();
    std::thread([&]() { Ct.Misc(); }).detach();
    std::thread([&]() { Ov.OverlayManager(); }).detach(); 

    Ov.OverlayLoop();

    Ov.DestroyOverlay();
    CloseHandle(m.pHandle);

    return 0;
}

void Overlay::OverlayLoop()
{
    g.GameHwnd = FindWindowA(nullptr, "EscapeFromTarkov");

    while (g.Run)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        HWND ForegroundWindow = GetForegroundWindow();
        if (ForegroundWindow == g.GameHwnd)
        {
            HWND TempProcessHwnd = GetWindow(ForegroundWindow, GW_HWNDPREV);
            SetWindowPos(Hwnd, TempProcessHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        Ct.RenderInfo();

        if (g.ShowMenu)
            Ct.RenderMenu();

        if (g.PlayerESP || g.ItemESP || g.ExfilESP)
            Ct.RenderESP();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);

    }
}