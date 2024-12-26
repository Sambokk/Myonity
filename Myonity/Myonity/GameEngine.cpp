#include "pch.h"
#include "GameEngine.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "EnginePreferences.h"
#include "Graphics.h"

GameEngine::GameEngine()
{

}

GameEngine::~GameEngine()
{

}

void GameEngine::Initialize(HWND Hwnd, HINSTANCE hInstance)
{
	m_HWnd = Hwnd;
	m_hInstance = hInstance;

	m_graphics = new Graphics();
	m_inputManager = new InputManager();
	m_timeManager = new TimeManager();

	m_graphics->Initialize(m_HWnd);
	m_inputManager->Initialize();
	m_timeManager->Initialize();
}

void GameEngine::Progress()
{
	m_inputManager->Update();
	float deltaTime = m_timeManager->Update();

	m_graphics->Render();
}

void GameEngine::Finalize()
{
	m_graphics->Finalize();

	delete m_graphics;
	delete m_inputManager;
	delete m_timeManager;
}
