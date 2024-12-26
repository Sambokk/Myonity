#pragma once

class GameEngine;

class Engine
{
///생성자와 소멸자--------------------------
public:
	Engine();
	~Engine();

	///멤버 함수--------------------------------
public:
	void Initialize(HINSTANCE hInstance);
	void Progress();
	void Finalize();

private:
	void initMainWindow();
	static LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

	///멤버 변수--------------------------------
private:
	HINSTANCE m_hInstance;
	HWND m_mainHWnd;
	WNDCLASS m_wndClass;

	GameEngine* m_gameEngine;
};

