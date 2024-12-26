#pragma once

class GameEngine;
class Graphics;

/// <summary>
/// ������ �ð��� ����
/// </summary>
class TimeManager
{
public:
	/// �����ڿ� �Ҹ��� ------------------------------------------------
	TimeManager();
	~TimeManager();


	/// �ɹ� �Լ� -----------------------------------------------------	
public:
	void Initialize();
	float Update();
	void DebugRender(Graphics* graphics);

	float GetDeltaTime() { return (float)m_deltaTime; }

	/// �ɹ� ���� -----------------------------------------------------
private:
	GameEngine* m_gameEngine;

	LARGE_INTEGER m_currentCount;
	LARGE_INTEGER m_prevCount;
	LARGE_INTEGER m_frequency;

	unsigned int m_frameCount;
	double m_frameTime;

	unsigned int m_fps;
	float m_deltaTime;
};