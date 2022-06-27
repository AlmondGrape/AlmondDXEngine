#pragma once

using namespace Microsoft::WRL;

namespace Almond
{
	struct WindowInfo;

	// 스왑체인
	class SwapChain
	{
	public:
		void Initialize(const WindowInfo& infoParam, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
		void Present();
		void SwapIndex();
		void Release();

		ComPtr<IDXGISwapChain> GetSwapChain() { return m_pSwapChain; }
		ComPtr<ID3D12Resource> GetRenderTarget(int32 index) { return m_pRTVBuffer[index]; }

		ComPtr<ID3D12Resource> GetBackRTVBuffer() { return m_pRTVBuffer[m_backBufferIndex]; }
		D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() { return m_RTVHandle[m_backBufferIndex]; }

	private:
		void CreateSwapChain(const WindowInfo& infoParam, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
		void CreateRTV(ComPtr<ID3D12Device> device);

	private:
		ComPtr<IDXGISwapChain>			m_pSwapChain;
		ComPtr<ID3D12Resource>			m_pRTVBuffer[SWAP_CHAIN_BUFFER_COUNT];
		ComPtr<ID3D12DescriptorHeap>	m_pRTVHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE		m_RTVHandle[SWAP_CHAIN_BUFFER_COUNT];

		uint32							m_backBufferIndex = 0;

	};

}
