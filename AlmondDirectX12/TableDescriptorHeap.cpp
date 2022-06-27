#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Graphics_DX12.h"

using namespace Almond;

void TableDescriptorHeap::Initialize(uint32 count)
{
	m_groupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	desc.NumDescriptors = count * REGISTER_COUNT;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDescHeap));

	m_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_groupSize = m_handleSize * REGISTER_COUNT;

}

void TableDescriptorHeap::Clear()
{
	m_currentGroupIndex = 0;
}

void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);
	uint32 destRange = 1;
	uint32 srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TableDescriptorHeap::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);
	uint32 destRange = 1;
	uint32 srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TableDescriptorHeap::CommitTable()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_pDescHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += m_currentGroupIndex * m_groupSize;
	CMD_LIST->SetGraphicsRootDescriptorTable(0, handle);
	m_currentGroupIndex++;
}

void TableDescriptorHeap::Release()
{
	m_handleSize = 0;
	m_groupSize = 0;
	m_groupCount = 0;
	m_currentGroupIndex = 0;
	m_pDescHeap.ReleaseAndGetAddressOf();
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint8>(reg));

}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(SRV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint8>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(uint8 reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_pDescHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += m_currentGroupIndex * m_groupSize;
	handle.ptr += reg * m_handleSize;
	return handle;
}