#include "Cheat/FrameCore.h"
#include "Framework/Overlay/Overlay.h"

Overlay*	C_Overlay = new Overlay;
CFramework* C_Tarkov  = new CFramework;

void Overlay::OverlayUserFunction()
{
	C_Tarkov->MiscAll();

	C_Tarkov->RenderInfo();

	if (g.g_ESP)
		C_Tarkov->RenderESP();

	if (g.ShowMenu)
		C_Tarkov->RenderMenu();
}

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