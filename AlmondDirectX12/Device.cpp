#include "pch.h"
#include "Device.h"

void Almond::Device::Initialize()
{
	// D3D12 ������� Ȱ��ȭ
#ifdef _DEBUG
	 D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebugController));
	 m_pDebugController->EnableDebugLayer();
#endif

	// DXGI (DirectX Graphics Infrastructure) ����
	//  - Direct3D�� �Բ� ���̴� API
	//  - ��ü ȭ�� ��� ��ȯ, ���� ���÷��� ��� ���� ��..
	CreateDXGIFactory(IID_PPV_ARGS(&m_pDxgi));

	// Device ����
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));

}

void Almond::Device::Release()
{
	m_pDebugController.ReleaseAndGetAddressOf();
	m_pDxgi.ReleaseAndGetAddressOf();
	m_pDevice.ReleaseAndGetAddressOf();
}
