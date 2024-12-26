#pragma once

class GameEngine;
class Graphics;

/// <summary>
/// 게임의 시간을 관리
/// </summary>
class TimeManager
{
public:
	/// 생성자와 소멸자 ------------------------------------------------
	TimeManager();
	~TimeManager();


	/// 맴버 함수 -----------------------------------------------------	
public:
	void Initialize();
	float Update();
	void DebugRender(Graphics* graphics);

	float GetDeltaTime() { return (float)m_deltaTime; }

	/// 맴버 변수 -----------------------------------------------------
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