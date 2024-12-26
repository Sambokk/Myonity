#pragma once
#include "pch.h"
#include "TimeManager.h"
#include "GameEngine.h"

TimeManager::TimeManager()
	:m_currentCount{}, m_prevCount{}, m_frequency{}
	, m_fps(), m_deltaTime(), m_frameCount(), m_frameTime(), m_gameEngine(nullptr)
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::Initialize()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_prevCount);
}

float TimeManager::Update()
{
	QueryPerformanceCounter(&m_currentCount);

	//deltaTime
	m_deltaTime = (m_currentCount.QuadPart - m_prevCount.QuadPart) / (float)m_frequency.QuadPart;
	m_prevCount = m_currentCount;

	//fps
	++m_frameCount;
	m_frameTime += m_deltaTime;

	if(m_frameTime >= 1.f)
	{
		m_fps = m_frameCount;
		m_frameCount = 0;
		m_frameTime = 0.0;
	}
	return m_deltaTime;
}

void TimeManager::DebugRender(Graphics* graphics)
{
	//wstring str = L"fps : " + std::to_wstring(m_fps);
	//graphics->DrawChar(str, { 10,10 });
}