#pragma once


class GraphicsWindow
{
/// 积己磊客 家戈磊 ------------------------------------------------
public:
	GraphicsWindow();
	~GraphicsWindow();
	
	
/// 干滚 窃荐 -----------------------------------------------------
public:
	void Initialize(HWND hwnd, ComPtr<ID3D11Device>	device, ComPtr<ID3D11DeviceContext>	context);

private:
	void InitializeDXGIFactory();
	void InitializeSwapChain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeDepthStencilBuffer();
	void InitializeStencilView();
	void InitializeRasterizerState();
	void InitializeViewPort();
	void InitializeSamplerState();
	void InitializeD2D();
	
/// 干滚 函荐 -----------------------------------------------------	
private:
	HWND m_hwnd = 0;
	D3D11_VIEWPORT m_viewport = {};

	unsigned int m_numerator = 0;
	unsigned int m_denominator = 0;

	ComPtr<ID3D11Device>			m_device;
	ComPtr<ID3D11DeviceContext>		m_context;

	ComPtr<IDXGISwapChain>			m_swapChain;
	ComPtr<IDXGIFactory>			m_factory;
	ComPtr<ID3D11RenderTargetView>	m_renderTargetView;
	ComPtr<ID3D11Texture2D>			m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView>	m_depthStencilView;


	ComPtr<ID3D11SamplerState>		m_samplerState;

	//d2d
	ComPtr<IDXGISurface> m_dxgiSurface;
	ComPtr<ID2D1Factory> m_d2dFactory;
	ComPtr<IDWriteFactory> m_d2dWriteFactory;
	ComPtr<ID2D1RenderTarget> m_d2dRenderTarget;
	std::unordered_map<UINT, ComPtr<IDWriteTextFormat>> m_textFormat;
};