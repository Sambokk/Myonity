#pragma once

class GraphicsWindow;
class RenderPass;
class GraphicsResourceManager;

class Graphics
{
///�����ڿ� �Ҹ���--------------------------
public:
	Graphics();
	~Graphics();

///��� �Լ�--------------------------------
public:
	void Initialize(HWND Hwnd);
	void Render();
	void Finalize();
	void Resize(HWND hwnd);

private:
	void CreateDevice();

///��� ����--------------------------------
private:
	RenderPass* m_renderPass = nullptr;
	GraphicsWindow* m_Window = nullptr;
	GraphicsResourceManager* m_resourceManager = nullptr;

	HWND m_Hwnd = 0;
	ComPtr<ID3D11Device>			m_device;
	ComPtr<ID3D11DeviceContext>		m_context;
};

