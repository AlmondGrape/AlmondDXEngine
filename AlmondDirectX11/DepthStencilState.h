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

		// �� ���Ľ� ������Ʈ (Z-���۸� On/Off)
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateZOff;

		// �������ֱ� ���� ���ؽ�Ʈ������ �����ص�... �ӽ����� ComPtr�� ��������?
		ComPtr<ID3D11DeviceContext> m_pD3DDeviceContext;
	};
}
