#include "Cheat/FrameCore.h"
#include "Framework/Overlay/Overlay.h"

Overlay*	C_Overlay = new Overlay;
CFramework* C_Tarkov  = new CFramework;

// DEBUG時にはコンソールウィンドウを表示する
#if _DEBUG
int main()
#else 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	// Fix DPI Scale
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

	// ウィンドウをベースにして初期化を行う
	if (!m.AttachProcess("EscapeFromTarkov", InitMode::WINDOW_TITLE)) // 詳細は Memory/Memory.h を参照
		return 1;

	// Overlay
	if (!C_Overlay->InitOverlay("EscapeFromTarkov", InitMode::WINDOW_TITLE)) // MemoryInitModeと同様
		return 2;

	// スレッドを作成
	std::thread([&]() { C_Tarkov->UpdateList(); }).detach();
	std::thread([&]() { C_Tarkov->UpdateStaticList(); }).detach();

	C_Overlay->OverlayLoop();
	C_Overlay->DestroyOverlay();
	m.DetachProcess();
	g.process_active = false;
	delete C_Tarkov, C_Overlay;

	return 0;
}

void Overlay::OverlayLoop()
{
	while (g.process_active)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Overlay and Misc
		C_Tarkov->MiscAll();
		OverlayManager();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Cheat
		C_Tarkov->RenderInfo();

		if (g.g_ESP)
			C_Tarkov->RenderESP();

		if (g.ShowMenu)
			C_Tarkov->RenderMenu();

		ImGui::Render();
		const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0);
	}
}