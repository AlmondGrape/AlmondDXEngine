#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Graphics_DX12.h"

void Almond::DepthStencilBuffer::Initialize(const WindowInfo& window, DXGI_FORMAT dsvFormat)
{
	m_dsvFormat = dsvFormat;

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(m_dsvFormat, window.width, window.height);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optimizedClearValue = CD3DX12_CLEAR_VALUE(m_dsvFormat, 1.0f, 0);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optimizedClearValue,
		IID_PPV_ARGS(&m_pDsvBuffer));

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pDsvHeap));

	m_dsvHandle = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
	DEVICE->CreateDepthStencilView(m_pDsvBuffer.Get(), nullptr, m_dsvHandle);

}

void Almond::DepthStencilBuffer::Release()
{
	m_dsvHandle = {};
	m_dsvFormat = {};
	m_pDsvBuffer.ReleaseAndGetAddressOf();
	m_pDsvHeap.ReleaseAndGetAddressOf();
}
