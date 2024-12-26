#include "pch.h"
#include "GraphicsWindow.h"
#include "DebugMacro.h"	

GraphicsWindow::GraphicsWindow()
{

}

GraphicsWindow::~GraphicsWindow()
{

}

void GraphicsWindow::Initialize(HWND hwnd, ComPtr<ID3D11Device>	device, ComPtr<ID3D11DeviceContext>	context)
{
	m_hwnd = hwnd;
	m_device = device;
	m_context = context;

	InitializeDXGIFactory();						// 수직동기화를 위한 해상도 정보 얻어오기
	InitializeSwapChain();							// 스왑체인 생성
	InitializeRenderTargetView();					// 랜더타겟 뷰 생성
	InitializeDepthBuffer();						// 깊이 버퍼용 텍스쳐 생성
	InitializeDepthStencilBuffer();					// 깊이 & 스텐실 생성
	InitializeStencilView();						// 깊이 버퍼 뷰 생성
	m_context->OMSetRenderTargets(					// 뷰들을 출력 병합기 단계에 묶기
		1, m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get());
	InitializeRasterizerState();					// 레스터라이즈 단계 설정
	InitializeViewPort();							// 뷰포트 설정
	InitializeSamplerState();						// 샘플러 상태 설정
	InitializeD2D();								// D2D 초기화

	// 화면 초기화
	float color[] = { 0.78f,0.886f,0.929f,1.f };
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), color);
	m_swapChain->Present(0, 0);
}

void GraphicsWindow::InitializeDXGIFactory()
{
	HRESULT hr;
	ComPtr<IDXGIAdapter> adapter;
	ComPtr<IDXGIOutput> adapterOutput;
	unsigned int numModes = 0;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	// dx11 그래픽 인터페이스 팩토리를 만든다.
	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)m_factory.GetAddressOf()));
	
	// factory로 기본 그래픽 인터페이스를 위한 adapter를 만든다
	HR(m_factory->EnumAdapters(0, adapter.GetAddressOf()));

	// 기본 adapter 출력 열거
	HR(adapter->EnumOutputs(0, adapterOutput.GetAddressOf()));

	// 어댑터 출력에 대한 DXGI_FORMAT_R8G8B8A_UNORM 표시 형식에 맞는 모드 수를 가져온다
	HR(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));

	// 모니터/비디오 카드 조합에 대해 가능한 모든 모드를 담을 목록
	displayModeList = new DXGI_MODE_DESC[numModes];

	// display mode list를 채운다
	HR(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList));

	for(UINT i = 0; i < numModes; ++i)
	{
		if(displayModeList[i].Width == (UINT)EnginePreferences::WINDOW_WIDTH
			&& displayModeList[i].Height == (UINT)EnginePreferences::WINDOW_HEIGHT)
		{
			m_numerator = displayModeList[i].RefreshRate.Numerator;
			m_denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	if(m_numerator == 0 && m_denominator == 0)
	{
		EnginePreferences::VSYNC_ENABLED = false;
	}

	// 메모리 해제
	delete[] displayModeList;
}

// 스왑체인(백버퍼) 초기화
void GraphicsWindow::InitializeSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	HRESULT hr;

	// 스왑체인 초기화
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = EnginePreferences::WINDOW_WIDTH;
	swapChainDesc.BufferDesc.Height = EnginePreferences::WINDOW_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 해상도를 변경할 때 디스플레이에 적용되는 이미지 스케일링 방식

	// 수직동기화 설정
	if(EnginePreferences::VSYNC_ENABLED)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_hwnd;
	swapChainDesc.SampleDesc.Count = 1;		//멀티 샘플링 off
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !EnginePreferences::FULL_SCREEN;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// 스왑체인 생성--------------------------------
	HR(m_factory->CreateSwapChain(
		m_device.Get(),
		&swapChainDesc,
		m_swapChain.GetAddressOf()
	));
}

void GraphicsWindow::InitializeRenderTargetView()
{
	HRESULT hr;

	// 백버퍼 포인터 받아오기
	ComPtr<ID3D11Texture2D> backBufferPtr;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBufferPtr.GetAddressOf()));

	// 랜더타겟뷰 생성
	HR(m_device->CreateRenderTargetView(backBufferPtr.Get(), 0, m_renderTargetView.GetAddressOf()));
}

void GraphicsWindow::InitializeDepthBuffer()
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = EnginePreferences::WINDOW_WIDTH;
	depthBufferDesc.Height = EnginePreferences::WINDOW_HEIGHT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  //DXGI_FORMAT_D24_UNORM_S8_UINT; DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1u;
	depthBufferDesc.SampleDesc.Quality = 0u;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	HR(m_device->CreateTexture2D(&depthBufferDesc, nullptr, m_depthStencilBuffer.GetAddressOf()));
}

void GraphicsWindow::InitializeDepthStencilBuffer()
{
	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 깊이 상태값 설정
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; //큐브맵을 위한

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//픽셀이 front-face일 때 stencil의 행동
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//픽셀이 back-face일 때 stencil의 행동
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이&스텐실 State 생성 및 bind
	ComPtr<ID3D11DepthStencilState> DSState;
	HR(m_device->CreateDepthStencilState(&depthStencilDesc, DSState.GetAddressOf()));
	m_context->OMSetDepthStencilState(DSState.Get(), 1u);

// 	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
// 	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
// 
// 	// 깊이 상태값 설정
// 	depthStencilDesc.DepthEnable = false;
// 	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
// 	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
// 
// 	depthStencilDesc.StencilEnable = true;
// 	depthStencilDesc.StencilReadMask = 0xFF;
// 	depthStencilDesc.StencilWriteMask = 0xFF;
// 
// 	//픽셀이 front-face일 때 stencil의 행동
// 	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
// 	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
// 	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
// 	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
// 
// 	//픽셀이 back-face일 때 stencil의 행동
// 	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
// 	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
// 	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
// 	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
// 
// 	// create disable state
// 	HR(m_device->CreateDepthStencilState(&depthStencilDesc, m_depthDisabledStensilState.GetAddressOf()));
}

void GraphicsWindow::InitializeStencilView()
{
	HRESULT hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	//desc 초기화
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  //깊이 버퍼용 텍스쳐량 같아야 한다.
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;

	// depth Stencil View  생성
	HR(m_device->CreateDepthStencilView(
		m_depthStencilBuffer.Get(),   // 초기화가 끝나면 없어도 된다
		&depthStencilViewDesc,
		m_depthStencilView.GetAddressOf()));
}

void GraphicsWindow::InitializeRasterizerState()
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC rasterDesc;

	// Desc초기화  
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;		// 어떤 면을 제거할것인가? NONE, FRONT, BACK
	rasterDesc.DepthBias = 0;					// 깊이값 조절(그림자 및 오브젝트 겹침에 사용) 음수일수록 뒤로간다
	rasterDesc.DepthBiasClamp = 0.0f;			// depthBias가 너무 큰 경우를 방지
	rasterDesc.DepthClipEnable = true;			// 깊이 클리핑을 활성화 또는 비활성화
	rasterDesc.FillMode = D3D11_FILL_SOLID;		// D3D11_FILL_WIREFRAME
	rasterDesc.FrontCounterClockwise = false;	// true : 정면은 시계 방향, false : 정면은 시계반대
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;			// 클리핑 사각형을 사용하여 클리핑
	rasterDesc.SlopeScaledDepthBias = 0.0f;		// 경사 기반의 깊이 바이어스 적용. 

	// 래스터라이즈 단계 생성
	ComPtr<ID3D11RasterizerState> cullBack;
	HR(m_device->CreateRasterizerState(&rasterDesc, cullBack.GetAddressOf()));

	// 래스터라이저 설정
	m_context->RSSetState(cullBack.Get());
}

void GraphicsWindow::InitializeViewPort()
{
	//후면영역 전체에 그리기
	m_viewport = {};
	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.Width = static_cast<float>(EnginePreferences::WINDOW_WIDTH);
	m_viewport.Height = static_cast<float>(EnginePreferences::WINDOW_HEIGHT);
	m_viewport.MinDepth = 0.f;    
	m_viewport.MaxDepth = 1.f;    

	m_context->RSSetViewports(1, &m_viewport);
}

void GraphicsWindow::InitializeD2D()
{
	HRESULT hr;

	//2d2용. 벡버퍼를 받아와서 랜더타겟을 설정해주자
	HR(m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_dxgiSurface.GetAddressOf())));
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_d2dFactory.GetAddressOf()));
	int dpi = GetDpiForSystem();

	RECT rc;
	GetClientRect(m_hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			static_cast<FLOAT>(dpi),
			static_cast<FLOAT>(dpi)
		);

	HR(m_d2dFactory->CreateDxgiSurfaceRenderTarget(
		m_dxgiSurface.Get(),
		&props,
		m_d2dRenderTarget.GetAddressOf()
	));

	//Direct2D를 사용하려면 IDXGISurface를 제공하는 Direct3D 디바이스를 
	//D3D10_CREATE_DEVICE_BGRA_SUPPORT 플래그로 만들어야 합니다.
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_d2dWriteFactory),
		reinterpret_cast<IUnknown**>(m_d2dWriteFactory.GetAddressOf())));
}

void GraphicsWindow::InitializeSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT hr;

	// 샘플러 desc 만들기
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;  //?
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// 샘플러  state 만들기
	hr = m_device->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
	if(FAILED(hr)) assert(false && L"Sampler State 생성 실패");

	// 샘플러 상태 설정
	m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}
