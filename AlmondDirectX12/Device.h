#pragma once
using namespace Microsoft::WRL;

namespace Almond
{
	class Device
	{
	public:
		void Initialize();
		void Release();

		ComPtr<IDXGIFactory> GetDXGI() { return m_pDxgi; }
		ComPtr<ID3D12Device> GetDevice() { return m_pDevice;}

	private:
		ComPtr<ID3D12Debug>		m_pDebugController;
		ComPtr<IDXGIFactory>	m_pDxgi;
		ComPtr<ID3D12Device>	m_pDevice;

	};
}

