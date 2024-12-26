#pragma once

class Graphics;
class InputManager;
class TimeManager;

/// <summary>
/// ������ ���� ����
/// </summary>
class GameEngine
{
	///�����ڿ� �Ҹ��� --------------------------------------------------
public:
	GameEngine();
	~GameEngine();

	///�ɹ� �Լ� --------------------------------------------------
	void Initialize(HWND Hwnd, HINSTANCE hInstance);
	void Progress();
	void Finalize();

	Graphics* GetGraphics() { return m_graphics; }
	InputManager* GetKeyManager() { return m_inputManager; }
	TimeManager* GetTimeManager() { return m_timeManager; }


	///�ɹ� ���� --------------------------------------------------
private:
	HWND m_HWnd = 0;
	HINSTANCE m_hInstance = 0;

	Graphics* m_graphics = nullptr;
	InputManager* m_inputManager = nullptr;
	TimeManager* m_timeManager = nullptr;
};