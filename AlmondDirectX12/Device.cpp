#include "pch.h"
#include "Device.h"

void Almond::Device::Initialize()
{
	// D3D12 디버그층 활성화
#ifdef _DEBUG
	 D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebugController));
	 m_pDebugController->EnableDebugLayer();
#endif

	// DXGI (DirectX Graphics Infrastructure) 생성
	//  - Direct3D와 함께 쓰이는 API
	//  - 전체 화면 모드 전환, 지원 디스플레이 모드 열거 등..
	CreateDXGIFactory(IID_PPV_ARGS(&m_pDxgi));

	// Device 생성
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));

}

void Almond::Device::Release()
{
	m_pDebugController.ReleaseAndGetAddressOf();
	m_pDxgi.ReleaseAndGetAddressOf();
	m_pDevice.ReleaseAndGetAddressOf();
}
