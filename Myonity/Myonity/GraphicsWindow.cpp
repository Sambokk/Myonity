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

	InitializeDXGIFactory();						// ��������ȭ�� ���� �ػ� ���� ������
	InitializeSwapChain();							// ����ü�� ����
	InitializeRenderTargetView();					// ����Ÿ�� �� ����
	InitializeDepthBuffer();						// ���� ���ۿ� �ؽ��� ����
	InitializeDepthStencilBuffer();					// ���� & ���ٽ� ����
	InitializeStencilView();						// ���� ���� �� ����
	m_context->OMSetRenderTargets(					// ����� ��� ���ձ� �ܰ迡 ����
		1, m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get());
	InitializeRasterizerState();					// �����Ͷ����� �ܰ� ����
	InitializeViewPort();							// ����Ʈ ����
	InitializeSamplerState();						// ���÷� ���� ����
	InitializeD2D();								// D2D �ʱ�ȭ

	// ȭ�� �ʱ�ȭ
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

	// dx11 �׷��� �������̽� ���丮�� �����.
	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)m_factory.GetAddressOf()));
	
	// factory�� �⺻ �׷��� �������̽��� ���� adapter�� �����
	HR(m_factory->EnumAdapters(0, adapter.GetAddressOf()));

	// �⺻ adapter ��� ����
	HR(adapter->EnumOutputs(0, adapterOutput.GetAddressOf()));

	// ����� ��¿� ���� DXGI_FORMAT_R8G8B8A_UNORM ǥ�� ���Ŀ� �´� ��� ���� �����´�
	HR(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));

	// �����/���� ī�� ���տ� ���� ������ ��� ��带 ���� ���
	displayModeList = new DXGI_MODE_DESC[numModes];

	// display mode list�� ä���
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

	// �޸� ����
	delete[] displayModeList;
}

// ����ü��(�����) �ʱ�ȭ
void GraphicsWindow::InitializeSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	HRESULT hr;

	// ����ü�� �ʱ�ȭ
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = EnginePreferences::WINDOW_WIDTH;
	swapChainDesc.BufferDesc.Height = EnginePreferences::WINDOW_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // �ػ󵵸� ������ �� ���÷��̿� ����Ǵ� �̹��� �����ϸ� ���

	// ��������ȭ ����
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
	swapChainDesc.SampleDesc.Count = 1;		//��Ƽ ���ø� off
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !EnginePreferences::FULL_SCREEN;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// ����ü�� ����--------------------------------
	HR(m_factory->CreateSwapChain(
		m_device.Get(),
		&swapChainDesc,
		m_swapChain.GetAddressOf()
	));
}

void GraphicsWindow::InitializeRenderTargetView()
{
	HRESULT hr;

	// ����� ������ �޾ƿ���
	ComPtr<ID3D11Texture2D> backBufferPtr;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBufferPtr.GetAddressOf()));

	// ����Ÿ�ٺ� ����
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

	// ���� ���°� ����
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; //ť����� ����

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//�ȼ��� front-face�� �� stencil�� �ൿ
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//�ȼ��� back-face�� �� stencil�� �ൿ
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ����&���ٽ� State ���� �� bind
	ComPtr<ID3D11DepthStencilState> DSState;
	HR(m_device->CreateDepthStencilState(&depthStencilDesc, DSState.GetAddressOf()));
	m_context->OMSetDepthStencilState(DSState.Get(), 1u);

// 	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
// 	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
// 
// 	// ���� ���°� ����
// 	depthStencilDesc.DepthEnable = false;
// 	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
// 	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
// 
// 	depthStencilDesc.StencilEnable = true;
// 	depthStencilDesc.StencilReadMask = 0xFF;
// 	depthStencilDesc.StencilWriteMask = 0xFF;
// 
// 	//�ȼ��� front-face�� �� stencil�� �ൿ
// 	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
// 	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
// 	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
// 	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
// 
// 	//�ȼ��� back-face�� �� stencil�� �ൿ
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

	//desc �ʱ�ȭ
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  //���� ���ۿ� �ؽ��ķ� ���ƾ� �Ѵ�.
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;

	// depth Stencil View  ����
	HR(m_device->CreateDepthStencilView(
		m_depthStencilBuffer.Get(),   // �ʱ�ȭ�� ������ ��� �ȴ�
		&depthStencilViewDesc,
		m_depthStencilView.GetAddressOf()));
}

void GraphicsWindow::InitializeRasterizerState()
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC rasterDesc;

	// Desc�ʱ�ȭ  
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;		// � ���� �����Ұ��ΰ�? NONE, FRONT, BACK
	rasterDesc.DepthBias = 0;					// ���̰� ����(�׸��� �� ������Ʈ ��ħ�� ���) �����ϼ��� �ڷΰ���
	rasterDesc.DepthBiasClamp = 0.0f;			// depthBias�� �ʹ� ū ��츦 ����
	rasterDesc.DepthClipEnable = true;			// ���� Ŭ������ Ȱ��ȭ �Ǵ� ��Ȱ��ȭ
	rasterDesc.FillMode = D3D11_FILL_SOLID;		// D3D11_FILL_WIREFRAME
	rasterDesc.FrontCounterClockwise = false;	// true : ������ �ð� ����, false : ������ �ð�ݴ�
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;			// Ŭ���� �簢���� ����Ͽ� Ŭ����
	rasterDesc.SlopeScaledDepthBias = 0.0f;		// ��� ����� ���� ���̾ ����. 

	// �����Ͷ����� �ܰ� ����
	ComPtr<ID3D11RasterizerState> cullBack;
	HR(m_device->CreateRasterizerState(&rasterDesc, cullBack.GetAddressOf()));

	// �����Ͷ����� ����
	m_context->RSSetState(cullBack.Get());
}

void GraphicsWindow::InitializeViewPort()
{
	//�ĸ鿵�� ��ü�� �׸���
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

	//2d2��. �����۸� �޾ƿͼ� ����Ÿ���� ����������
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

	//Direct2D�� ����Ϸ��� IDXGISurface�� �����ϴ� Direct3D ����̽��� 
	//D3D10_CREATE_DEVICE_BGRA_SUPPORT �÷��׷� ������ �մϴ�.
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_d2dWriteFactory),
		reinterpret_cast<IUnknown**>(m_d2dWriteFactory.GetAddressOf())));
}

void GraphicsWindow::InitializeSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT hr;

	// ���÷� desc �����
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

	// ���÷�  state �����
	hr = m_device->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
	if(FAILED(hr)) assert(false && L"Sampler State ���� ����");

	// ���÷� ���� ����
	m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}
