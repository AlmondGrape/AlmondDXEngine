#pragma once
using namespace Microsoft::WRL;

namespace Almond
{
	struct WindowInfo;
	class Device
	{
	public:
		Device() = default;
		~Device() = default;
		Device(const Device& other) = default;
		Device& operator =(const Device& other) = default;
		Device(Device&& other) = default;

	public:
		void Initialize(WindowInfo& info);
		void Release();

		ComPtr<IDXGIFactory> GetDXGI() { return m_pDxgi; }
		ComPtr<ID3D11Device> GetDevice() { return m_pD3DDevice; }
		ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_pD3DDeviceContext; }

	private:

		ComPtr<IDXGIFactory>		m_pDxgi;
		ComPtr<ID3D11Device>		m_pD3DDevice;
		ComPtr<ID3D11DeviceContext> m_pD3DDeviceContext;

	};
}

