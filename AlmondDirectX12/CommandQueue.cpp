#include "pch.h"
#include "CommandQueue.h"
#include "Graphics_DX12.h"

using namespace Almond;

CommandQueue::~CommandQueue()
{
	// �̺�Ʈ �ڵ��� �ݾ���.
	CloseHandle(m_fenceEvent);
}

void CommandQueue::Initialize(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	m_pSwapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// 6. Ŀ�ǵ� ť ����
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCmdQueue));

	// 7. Ŀ�ǵ� Allocator ����
	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU�� ���� �����ϴ� ��� ���
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdAlloc));

	// A6. Ŀ�ǵ� ����Ʈ ����
	// GPU�� �ϳ��� �ý��ۿ����� nodeMask�� 0����.. Direct or Bundle
	device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCmdAlloc.Get(),
		nullptr,
		IID_PPV_ARGS(&m_pCmdList));

	// A7. Ŀ�ǵ� ����Ʈ �ݱ�
	m_pCmdList->Close();

	// A10. �潺 �����
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	// fence point���� ����� ǥ���ϱ� ���� fence value�� ������Ŵ
	m_fenceValue++;

	// ��� ��⿭�� ����� �߰��Ͽ�, �� fence point�� ����
	//   GPU Timeline ���� �����Ƿ�, 
	//   �� Signal ���� ��� ����� ó�� �Ϸ� �� �� ���� �� fence point�� �������� ����.
	m_pCmdQueue->Signal(m_pFence.Get(), m_fenceValue);

	// A12. GPU�� �� Fence Point���� ����� ó�� �Ϸ� �� �� ���� ��ٸ�.
	if (m_pFence->GetCompletedValue() < m_fenceValue)
	{
		m_pFence->SetEventOnCompletion(m_fenceValue, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

void CommandQueue::BeginRender(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	// r1. Command List Allocator ����
	m_pCmdAlloc->Reset();
	// r2. Command List ����
	m_pCmdList->Reset(m_pCmdAlloc.Get(), nullptr);
	// r3. Graphic Root Signature ����
	m_pCmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());
	// r4. Set the viewport and scissor rect.  	
	m_pCmdList->RSSetViewports(1, vp);
	m_pCmdList->RSSetScissorRects(1, rect);

	// r5. Set a resource barrier & 
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pSwapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,			// ȭ�� ���
		D3D12_RESOURCE_STATE_RENDER_TARGET);	// ���� �����
	m_pCmdList->ResourceBarrier(1, &barrier);	

	TABLE_DESC_HEAP->Clear();
	ID3D12DescriptorHeap* descHeap = TABLE_DESC_HEAP->GetDescriptorHeap().Get();
	m_pCmdList->SetDescriptorHeaps(1, &descHeap);
	
	// Specify the buffers we are going to render to. (indicating the back buffer is to be used as a render target)
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = m_pSwapChain->GetBackRTV();
	m_pCmdList->ClearRenderTargetView(backBufferView, DirectX::Colors::LightSteelBlue, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = DEPTHSTENCIL_BUFFER->GetDSVCpuHandle();
	m_pCmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);

	m_pCmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ��� ���� Ŭ����
	CONSTANT_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();	
	//CONSTANT_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();

}

void CommandQueue::EndRender()
{
	// r6. Record commands into the command list.�� �������� Render() �Լ�����..
	// r7. Indicate the back buffer will be used to present after the command list has executed.
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pSwapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,			// ȭ�� ���
		D3D12_RESOURCE_STATE_RENDER_TARGET);	// ���� �����
	m_pCmdList->ResourceBarrier(1, &barrier);
	// r8. Close the command list to further recording.
	m_pCmdList->Close();

	// R2. Execute the command list.
	ID3D12CommandList* cmdListArr[] = { m_pCmdList.Get() };
	m_pCmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	// R3. Present the frame
	m_pSwapChain->Present();

	// R4. Wait for the GPU to finish
	WaitSync();

	m_pSwapChain->SwapIndex();
}

void CommandQueue::FlushResourceCommandQueue()
{
	m_pResCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { m_pResCmdList.Get() };
	m_pCmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	m_pResCmdAlloc->Reset();
	m_pResCmdList->Reset(m_pResCmdAlloc.Get(), nullptr);

}

void CommandQueue::Release()
{
	m_pCmdQueue.ReleaseAndGetAddressOf();
	m_pCmdAlloc.ReleaseAndGetAddressOf();
	m_pCmdList.ReleaseAndGetAddressOf();
	m_pResCmdAlloc.ReleaseAndGetAddressOf();
	m_pResCmdList.ReleaseAndGetAddressOf();
	m_pFence.ReleaseAndGetAddressOf();
	m_pSwapChain.reset();
}
