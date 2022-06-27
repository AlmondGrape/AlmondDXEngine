#pragma once

using namespace Microsoft::WRL;

namespace Almond
{
	struct WindowInfo;
	class DepthStencilBuffer
	{
	public:
		void Initialize(const WindowInfo& window, DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT);
		void Release();

		D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuHandle() { return m_dsvHandle; }
		DXGI_FORMAT GetDSVFormat() { return m_dsvFormat; }

	private:
		// Depth Stencil View
		ComPtr<ID3D12Resource>				m_pDsvBuffer;
		ComPtr<ID3D12DescriptorHeap>		m_pDsvHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE			m_dsvHandle = {};
		DXGI_FORMAT							m_dsvFormat = {};

	};

}
