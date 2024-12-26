#include "pch.h"
#include "Engine.h"
#include "EnginePreferences.h"
#include "resource.h"
#include "GameEngine.h"

#include "imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


Engine::Engine()
	:m_hInstance(), m_mainHWnd(), m_wndClass(), m_gameEngine(nullptr)
{

}

Engine::~Engine()
{

}

void Engine::Initialize(HINSTANCE hInstance)
{
	m_hInstance = hInstance;

	initMainWindow();

	m_gameEngine = new GameEngine();
	m_gameEngine->Initialize(m_mainHWnd, hInstance);
}

void Engine::Progress()
{
	MSG msg;

	while(TRUE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg); //키 입력 받기
			DispatchMessage(&msg); // 메세지를 윈도우 프로시저로 전달하여 처리
		}

		m_gameEngine->Progress();
	}
}

void Engine::Finalize()
{
	m_gameEngine->Finalize();
	RELEASE_PTR(m_gameEngine);
}

LRESULT CALLBACK Engine::MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch(message)
	{
	case WM_SIZE:
		if(wParam == SIZE_MINIMIZED)
			return 0;
		// 		g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		// 		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;

	case WM_SYSCOMMAND:
		if((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;

		///TODO::리사이즈?

	}
	return ::DefWindowProcW(hWnd, message, wParam, lParam);
}

void Engine::initMainWindow()
{
	m_wndClass.style = CS_HREDRAW | CS_VREDRAW;
	m_wndClass.lpfnWndProc = MainWndProc;
	m_wndClass.cbClsExtra = 0;
	m_wndClass.cbWndExtra = 0;
	m_wndClass.hInstance = m_hInstance;
	m_wndClass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
	//wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wndClass.hCursor = LoadCursor(m_hInstance, (LPCWSTR)IDC_CURSOR1);
	m_wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_wndClass.lpszMenuName = NULL;
	m_wndClass.lpszClassName = EnginePreferences::TITLE_NAME;

	ShowCursor(EnginePreferences::IS_CURSOR_SHOW);

	// 윈도 클래스 등록
	RegisterClass(&m_wndClass);

	// 윈도 생성
	m_mainHWnd = CreateWindowEx(
		0,								// 확장 스타일 ( WS_EX_TOPMOST : 항상 가장 위로 / WS_EX_TRANSPARENT: 투명한 창
		EnginePreferences::TITLE_NAME,	// 창 클래스 이름 (szWindowClass)
		EnginePreferences::TITLE_NAME,	// 창 제목 ( szTitle)
		WS_OVERLAPPEDWINDOW,			// WS_POPUP 
		50,								//x 
		50,								//y
		EnginePreferences::WINDOW_WIDTH,	//너비
		EnginePreferences::WINDOW_HEIGHT,	//높이
		NULL,							// 부모 창 핸들
		NULL,							// 메뉴 핸들
		m_hInstance,					// 인스턴스 핸들
		NULL							// 추가 어플리케이션 데이터
	);

	//투명한 창 만들기
	DWORD dwExStyle = WS_EX_LAYERED;
	SetWindowLong(m_mainHWnd, GWL_EXSTYLE, dwExStyle);
	SetLayeredWindowAttributes(m_mainHWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

	//윈도우 크기 정확하게 조정
	RECT rc = { 0,0,EnginePreferences::WINDOW_WIDTH, EnginePreferences::WINDOW_HEIGHT };
	AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// 창의 크기 설정
	SetWindowPos(m_mainHWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

	// 생성된 윈도를 화면에 표시
	ShowWindow(m_mainHWnd, SW_SHOWNORMAL);
	UpdateWindow(m_mainHWnd);
}
