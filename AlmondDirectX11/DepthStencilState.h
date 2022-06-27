#pragma once

using namespace Microsoft::WRL;

namespace Almond
{
	class Device;
	class DepthStencilState
	{
	public:
		DepthStencilState() = default;
		~DepthStencilState() = default;
		DepthStencilState(const DepthStencilState& other) = default;
		DepthStencilState& operator =(const DepthStencilState& other) = default;
		DepthStencilState(DepthStencilState&& other) = default;

	public:
		void Initialize(std::shared_ptr<Device> device);
		void Release();

		void SetDepthEnableState();
		void SetDepthDisableState();

	private:		

		// 댑스 스탠실 스테이트 (Z-버퍼링 On/Off)
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateZOff;

		// 셋팅해주기 위해 컨텍스트포인터 저장해둠... 임시참조 ComPtr은 어케하지?
		ComPtr<ID3D11DeviceContext> m_pD3DDeviceContext;
	};
}
