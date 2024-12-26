#pragma once

class Graphics;
class InputManager;
class TimeManager;

/// <summary>
/// 게임의 메인 루프
/// </summary>
class GameEngine
{
	///생성자와 소멸자 --------------------------------------------------
public:
	GameEngine();
	~GameEngine();

	///맴버 함수 --------------------------------------------------
	void Initialize(HWND Hwnd, HINSTANCE hInstance);
	void Progress();
	void Finalize();

	Graphics* GetGraphics() { return m_graphics; }
	InputManager* GetKeyManager() { return m_inputManager; }
	TimeManager* GetTimeManager() { return m_timeManager; }


	///맴버 변수 --------------------------------------------------
private:
	HWND m_HWnd = 0;
	HINSTANCE m_hInstance = 0;

	Graphics* m_graphics = nullptr;
	InputManager* m_inputManager = nullptr;
	TimeManager* m_timeManager = nullptr;
};