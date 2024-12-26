#include "EnginePreferences.h"
#include "pch.h"
#include "Engine.h"
#include <crtdbg.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR	lpCmdLine,
	_In_ int nCmdShow)
{
	// 메모리 누수 검사
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//ReportLiveObjects();
	//_CrtSetBreakAlloc(301);

	Engine* engine = new Engine();
	engine->Initialize(hInstance);
	engine->Progress();
	engine->Finalize();
	delete engine;

	return 0;
}