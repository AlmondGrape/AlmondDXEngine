#include "pch.h"
#include "Graphics_DX11.h"

using namespace Almond;

shared_ptr<Graphics_DX11> Graphics_DX11::s_pGraphics_DX11 = nullptr;

void Graphics_DX11::Initialize(HWND hWnd, int width, int height)
{
	HR(CoInitialize(NULL));

	m_windowInfo.hWnd = hWnd;
	m_windowInfo.width = width;
	m_windowInfo.height = height;
	m_windowInfo.enable4xMSAA = true;

	s_pGraphics_DX11 = dynamic_pointer_cast<Graphics_DX11>(IGraphics::GetInstance());

	// DirectX 리소스 인스턴스 생성 및 초기화
	m_pDevice = make_shared<Device>();
	m_pRasterizerState = make_shared<RasterizerState>();
	m_pSwapChain = make_shared<SwapChain>();
	m_pDepthStencilState = make_shared<DepthStencilState>();

	m_pDevice->Initialize(m_windowInfo);
	m_pRasterizerState->Initialize(m_pDevice->GetDevice());
	m_pSwapChain->Initialize(m_pDevice, m_windowInfo);
	m_pDepthStencilState->Initialize(m_pDevice);

}

void Graphics_DX11::BeginRender()
{
	// 백버퍼에 그릴 준비, 스테이트를 솔리드로..
	m_pSwapChain->BeginRenderToBackBuffer(m_pDevice->GetDeviceContext());
	m_pRasterizerState->SetSolidState(m_pDevice->GetDeviceContext());

}

void Graphics_DX11::Render()
{

}

void Graphics_DX11::EndRender()
{
	m_pDepthStencilState->SetDepthEnableState();
	m_pSwapChain->Present();
}

void Graphics_DX11::Release()
{
	m_pDevice->Release();
	m_pRasterizerState->Release();
	m_pSwapChain->Release();
	m_pDepthStencilState->Release();

}

void Graphics_DX11::ResizeWindow(int32 width, int32 height)
{
	m_windowInfo.width = width;
	m_windowInfo.height = height;

	// 윈도우 사이즈에 관련된 리소스 재설정
	m_pSwapChain->SetWindowDependentResource(m_pDevice, width, height);

	// 뷰포트 재설정.
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = static_cast<float>(width);
	m_viewPort.Height = static_cast<float>(height);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	m_pDevice->GetDeviceContext()->RSSetViewports(1, &m_viewPort);
	
	// 카메라 트랜스폼도... 여기서?

}
