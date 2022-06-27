#include "pch.h"
#include "ConstantBuffer.h"
#include "Graphics_DX12.h"

using namespace Almond;

ConstantBuffer::~ConstantBuffer()
{
	if (m_pCbvBuffer)
	{
		if (m_pCbvBuffer != nullptr)
			m_pCbvBuffer->Unmap(0, nullptr);

		m_pCbvBuffer = nullptr;
	}
}

void ConstantBuffer::Initialize(CBV_REGISTER reg, uint32 size, uint32 count)
{
	m_reg = reg;

	// ��� ���۴� 256 ����Ʈ ����� ������ ��.
	// & ~255 (������ ���� 8��Ʈ�� �� �� ����ٴ� ��..)
	// size + 255 (��� �����͸� ���� �� �ֵ��� �ݿø� �Ѵٴ� ��)
	m_elementSize = (size + 255) & ~255;
	m_elementCount = count;

	CreateBuffer();
	CreateView();
}

void ConstantBuffer::Clear()
{
	m_currentIndex = 0;
}

void ConstantBuffer::PushData(void* buffer, uint32 size)
{
	// ó�� �ʱ�ȭ �� �ͺ��� ���� ����ϸ� crash..
	assert(m_currentIndex < m_elementCount);
	assert(m_elementSize == ((size + 255) & ~255));

	// GPU �޸𸮿� ���ε� ������ ������ ���� & GPU�� PUSH
	memcpy(&m_pMappedBuffer[m_currentIndex * m_elementSize], buffer, size);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCpuHandle(m_currentIndex);
	TABLE_DESC_HEAP->SetCBV(cpuHandle, m_reg);

	m_currentIndex++;

}

void ConstantBuffer::Release()
{
	m_pCbvBuffer.ReleaseAndGetAddressOf();
	m_pCbvHeap.ReleaseAndGetAddressOf();
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = m_pCbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * m_elementSize;
	return objCBAddress;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::GetCpuHandle(uint32 index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cpuHandleBegin, index * m_handleIncrementSize);
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = m_elementSize * m_elementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pCbvBuffer));

	m_pCbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_pMappedBuffer));
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).

}

void ConstantBuffer::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = m_elementCount;
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DEVICE->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&m_pCbvHeap));

	m_cpuHandleBegin = m_pCbvHeap->GetCPUDescriptorHandleForHeapStart();
	m_handleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (uint32 i = 0; i < m_elementCount; ++i)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuHandle(i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_pCbvBuffer->GetGPUVirtualAddress() + static_cast<uint64>(m_elementSize) * i;
		cbvDesc.SizeInBytes = m_elementSize;   // CB size is required to be 256-byte aligned.

		DEVICE->CreateConstantBufferView(&cbvDesc, cbvHandle);
	}
}
