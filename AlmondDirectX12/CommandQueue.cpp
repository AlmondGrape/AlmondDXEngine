#include "pch.h"
#include "CommandQueue.h"
#include "Graphics_DX12.h"

using namespace Almond;

CommandQueue::~CommandQueue()
{
	// 이벤트 핸들을 닫아줌.
	CloseHandle(m_fenceEvent);
}

void CommandQueue::Initialize(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	m_pSwapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// 6. 커맨드 큐 생성
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCmdQueue));

	// 7. 커맨드 Allocator 생성
	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU가 직접 실행하는 명령 목록
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdAlloc));

	// A6. 커맨드 리스트 생성
	// GPU가 하나인 시스템에서는 nodeMask를 0으로.. Direct or Bundle
	device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCmdAlloc.Get(),
		nullptr,
		IID_PPV_ARGS(&m_pCmdList));

	// A7. 커맨드 리스트 닫기
	m_pCmdList->Close();

	// A10. 펜스 만들기
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	// fence point까지 명령을 표시하기 위해 fence value를 진전시킴
	m_fenceValue++;

	// 명령 대기열에 명령을 추가하여, 새 fence point를 설정
	//   GPU Timeline 내에 있으므로, 
	//   이 Signal 이전 모든 명령이 처리 완료 될 때 까지 새 fence point가 설정되지 않음.
	m_pCmdQueue->Signal(m_pFence.Get(), m_fenceValue);

	// A12. GPU가 이 Fence Point까지 명령을 처리 완료 할 때 까지 기다림.
	if (m_pFence->GetCompletedValue() < m_fenceValue)
	{
		m_pFence->SetEventOnCompletion(m_fenceValue, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

void CommandQueue::BeginRender(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	// r1. Command List Allocator 리셋
	m_pCmdAlloc->Reset();
	// r2. Command List 리셋
	m_pCmdList->Reset(m_pCmdAlloc.Get(), nullptr);
	// r3. Graphic Root Signature 셋팅
	m_pCmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());
	// r4. Set the viewport and scissor rect.  	
	m_pCmdList->RSSetViewports(1, vp);
	m_pCmdList->RSSetScissorRects(1, rect);

	// r5. Set a resource barrier & 
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pSwapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,			// 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET);	// 외주 결과물
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

	// 상수 버퍼 클리어
	CONSTANT_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();	
	//CONSTANT_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();

}

void CommandQueue::EndRender()
{
	// r6. Record commands into the command list.는 랜더러의 Render() 함수에서..
	// r7. Indicate the back buffer will be used to present after the command list has executed.
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pSwapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,			// 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET);	// 외주 결과물
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
