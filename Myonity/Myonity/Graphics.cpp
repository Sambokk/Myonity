#include "pch.h"
#include "Graphics.h"

#include "GraphicsResourceManager.h"
#include "RenderPass.h"
#include "GraphicsWindow.h"
#include "ErrorLogger.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

void Graphics::Initialize(HWND Hwnd)
{
	m_Hwnd = Hwnd;
	
	CreateDevice();

#ifdef _DEBUG
	ErrorLogger::GetInstance()->Initialize();
#endif

	m_Window = new GraphicsWindow();
	m_renderPass = new RenderPass();
	m_resourceManager = new GraphicsResourceManager();

	m_Window->Initialize(m_Hwnd, m_device, m_context);
	m_resourceManager->Initialize();
	m_renderPass->Initialize();
}

void Graphics::Render()
{

}

void Graphics::Finalize()
{
	delete m_renderPass;
	delete m_resourceManager;
	delete m_Window;
}

void Graphics::Resize(HWND hwnd)
{

}

void Graphics::CreateDevice()
{
	// D2D와 DX11을 같이 쓰려면 이 flag가 필요하다.
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_1
	};

	hr = D3D11CreateDevice(
		0,								// 기본 어댑터
		D3D_DRIVER_TYPE_HARDWARE,		// 3차원 그래픽 가속 사용
		0,								// 소프트웨어 장치를 사용하지 않음
		createDeviceFlags,				// 장치 생성 플래그 지정
		0, 0,							// 기본 기능 수준 배열(항상 LEVEL 11이 선택되게 함)
		D3D11_SDK_VERSION,
		m_device.GetAddressOf(),
		featureLevels,
		m_context.GetAddressOf()
	);

	if(FAILED(hr))
	{
		assert(false && "D3D11CreateDevice Failed");
	}
}
