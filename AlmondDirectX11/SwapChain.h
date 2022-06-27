#pragma once

using namespace Microsoft::WRL;

namespace Almond
{
	struct WindowInfo;
	class RenderTarget;
	class Device;
	class SwapChain
	{
	public:
		SwapChain() = default;
		~SwapChain() = default;
		SwapChain(const SwapChain& other) = default;
		SwapChain& operator =(const SwapChain& other) = default;
		SwapChain(SwapChain&& other) = default;

	public:
		void Initialize(std::shared_ptr<Device> device, WindowInfo& info);
		void Release();

		void BeginRenderToBackBuffer(ComPtr<ID3D11DeviceContext> context);
		void Present();
		void SetWindowDependentResource(std::shared_ptr<Device> device, int screenWidth, int screenHeight);


	private:
		ComPtr<IDXGISwapChain> m_pSwapChain;
		ComPtr<IDXGISwapChain1> m_pSwapChain1;

		std::shared_ptr<RenderTarget> m_pBackBuffer;
	};
}
