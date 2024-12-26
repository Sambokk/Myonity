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

			TranslateMessage(&msg); //Ű �Է� �ޱ�
			DispatchMessage(&msg); // �޼����� ������ ���ν����� �����Ͽ� ó��
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

		///TODO::��������?

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

	// ���� Ŭ���� ���
	RegisterClass(&m_wndClass);

	// ���� ����
	m_mainHWnd = CreateWindowEx(
		0,								// Ȯ�� ��Ÿ�� ( WS_EX_TOPMOST : �׻� ���� ���� / WS_EX_TRANSPARENT: ������ â
		EnginePreferences::TITLE_NAME,	// â Ŭ���� �̸� (szWindowClass)
		EnginePreferences::TITLE_NAME,	// â ���� ( szTitle)
		WS_OVERLAPPEDWINDOW,			// WS_POPUP 
		50,								//x 
		50,								//y
		EnginePreferences::WINDOW_WIDTH,	//�ʺ�
		EnginePreferences::WINDOW_HEIGHT,	//����
		NULL,							// �θ� â �ڵ�
		NULL,							// �޴� �ڵ�
		m_hInstance,					// �ν��Ͻ� �ڵ�
		NULL							// �߰� ���ø����̼� ������
	);

	//������ â �����
	DWORD dwExStyle = WS_EX_LAYERED;
	SetWindowLong(m_mainHWnd, GWL_EXSTYLE, dwExStyle);
	SetLayeredWindowAttributes(m_mainHWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

	//������ ũ�� ��Ȯ�ϰ� ����
	RECT rc = { 0,0,EnginePreferences::WINDOW_WIDTH, EnginePreferences::WINDOW_HEIGHT };
	AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// â�� ũ�� ����
	SetWindowPos(m_mainHWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

	// ������ ������ ȭ�鿡 ǥ��
	ShowWindow(m_mainHWnd, SW_SHOWNORMAL);
	UpdateWindow(m_mainHWnd);
}
