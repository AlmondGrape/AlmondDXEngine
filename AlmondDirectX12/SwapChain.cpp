#include "pch.h"
#include "SwapChain.h"
#include "Graphics_DX12.h"

using namespace Almond;

void SwapChain::Initialize(const WindowInfo& infoParam, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	CreateSwapChain(infoParam, dxgi, cmdQueue);
	CreateRTV(device);

}

void SwapChain::Present()
{
	// 프레임을 그려줌.	
	m_pSwapChain->Present(0, 0);
}

void SwapChain::SwapIndex()
{
	m_backBufferIndex = (m_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void SwapChain::Release()
{
	m_backBufferIndex = 0;
	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		m_pRTVBuffer[i].ReleaseAndGetAddressOf();

	m_pRTVHeap.ReleaseAndGetAddressOf();
	m_pSwapChain.ReleaseAndGetAddressOf();
}

void SwapChain::CreateSwapChain(const WindowInfo& infoParam, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// 이전에 만든 정보를 날림
	m_pSwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<uint32>(infoParam.width); // 버퍼의 해상도 너비
	sd.BufferDesc.Height = static_cast<uint32>(infoParam.height); // 버퍼의 해상도 높이
	sd.BufferDesc.RefreshRate.Numerator = 60; // 화면 갱신 비율
	sd.BufferDesc.RefreshRate.Denominator = 1; // 화면 갱신 비율
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 디스플레이 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // 멀티 샘플링 OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 후면 버퍼에 렌더링할 것 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // 전면+후면 버퍼
	sd.OutputWindow = infoParam.hWnd;
	sd.Windowed = infoParam.windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 전면 후면 버퍼 교체 시 이전 프레임 정보 버림
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &m_pSwapChain);

	for (int32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pRTVBuffer[i]));
}

void SwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	// Descripter(DX12) = View (~DX11)
	// DX12에서는 [Descripter Heap] 으로 RTV를 생성한다.

	int32 rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;

	// 같은 종류의 데이터끼리 배열로 관리
	// RTV 목록 : [ ] [ ]
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&m_pRTVHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = m_pRTVHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		m_RTVHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		device->CreateRenderTargetView(m_pRTVBuffer[i].Get(), nullptr, m_RTVHandle[i]);
	}

}
