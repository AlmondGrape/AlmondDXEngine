#include "pch.h"
#include "Graphics_DX12.h"

using namespace Almond;

// static ���� �ʱ�ȭ.
shared_ptr<Graphics_DX12> Graphics_DX12::s_pGraphics_DX12 = nullptr;

void Graphics_DX12::Initialize(HWND hWnd, int width, int height)
{
	m_windowInfo.hWnd = hWnd;
	m_windowInfo.width = width;
	m_windowInfo.height = height;
	m_windowInfo.windowed = true; // �ӽ�

	// �ν��Ͻ��� ���� abstract�� function�� ����.
	s_pGraphics_DX12 = dynamic_pointer_cast<Graphics_DX12>(IGraphics::GetInstance());
	
	// ���������� & ���� �ν��Ͻ� ���� & �ʱ�ȭ
	m_pDevice = make_shared<Device>();
	m_pCmdQueue = make_shared<CommandQueue>();
	m_pSwapChain = make_shared<SwapChain>();
	m_pRootSignature = make_shared<RootSignature>();
	m_pTableDescriptorHeap = make_shared<TableDescriptorHeap>();
	m_pDepthStencilBuffer = make_shared<DepthStencilBuffer>();

	m_pDevice->Initialize();
	m_pCmdQueue->Initialize(m_pDevice->GetDevice(), m_pSwapChain);
	m_pSwapChain->Initialize(m_windowInfo, m_pDevice->GetDevice(), m_pDevice->GetDXGI(), m_pCmdQueue->GetCmdQueue());
	m_pRootSignature->Initialize(m_pDevice->GetDevice());
	m_pTableDescriptorHeap->Initialize(256);
	m_pDepthStencilBuffer->Initialize(m_windowInfo);

	// ������� ����..
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(Transform), 256);

	// �׷��� ȭ�� ����
	ResizeWindow(m_windowInfo.width, m_windowInfo.height);
	m_viewPort = { 0, 0, static_cast<FLOAT>(m_windowInfo.width), static_cast<FLOAT>(m_windowInfo.height), 0.0f, 1.0f };
	m_scissorRect = CD3DX12_RECT(0, 0, m_windowInfo.width, m_windowInfo.height);

}

void Graphics_DX12::BeginRender()
{
	m_pCmdQueue->BeginRender(&m_viewPort, &m_scissorRect);
}

void Graphics_DX12::Render()
{
	// todo: ���� ������ �������� ���� �ν��Ͻ��� �ֹ��޾� �׷��߰���..

}

void Graphics_DX12::EndRender()
{
	m_pCmdQueue->EndRender();
}

void Graphics_DX12::Release()
{
	m_pDevice->Release();
	m_pCmdQueue->Release();
	m_pSwapChain->Release();
	m_pRootSignature->Release();
	m_pTableDescriptorHeap->Release();
	m_pDepthStencilBuffer->Release();
	CoUninitialize();
}

void Graphics_DX12::ResizeWindow(int32 width, int32 height)
{
	m_windowInfo.width = width;
	m_windowInfo.height = height;

	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_windowInfo.hWnd, 0, 100, 100, width, height, 0);

	// �𽺽��Ľ� ���� �ٽ� ������ֱ�..
	m_pDepthStencilBuffer->Release();
	m_pDepthStencilBuffer->Initialize(m_windowInfo);

}

void Graphics_DX12::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	uint8 typeInt = static_cast<uint8>(reg);
	assert(m_pConstantBuffers.size() == typeInt);

	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Initialize(reg, bufferSize, count);
	m_pConstantBuffers.push_back(buffer);
}
