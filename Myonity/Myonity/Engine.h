#pragma once

class GameEngine;

class Engine
{
///�����ڿ� �Ҹ���--------------------------
public:
	Engine();
	~Engine();

	///��� �Լ�--------------------------------
public:
	void Initialize(HINSTANCE hInstance);
	void Progress();
	void Finalize();

private:
	void initMainWindow();
	static LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

	///��� ����--------------------------------
private:
	HINSTANCE m_hInstance;
	HWND m_mainHWnd;
	WNDCLASS m_wndClass;

	GameEngine* m_gameEngine;
};

