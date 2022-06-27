#pragma once

using namespace Microsoft::WRL;
using namespace std;

namespace Almond
{
	class SwapChain;
	class CommandQueue
	{
	public:
		~CommandQueue();

		void Initialize(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
		void WaitSync();
		void BeginRender(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
		void EndRender();
		void Release();

		void FlushResourceCommandQueue();

		ComPtr<ID3D12CommandQueue> GetCmdQueue() { return m_pCmdQueue; }
		ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return m_pCmdList; }

	private:
		// ť ��Ͽ� �ϰ��� ����ߴ� �ѹ濡 ��û�ؼ� ó��
		ComPtr<ID3D12CommandQueue>			m_pCmdQueue;

		ComPtr<ID3D12CommandAllocator>		m_pCmdAlloc;
		ComPtr<ID3D12GraphicsCommandList>	m_pCmdList;
		ComPtr<ID3D12CommandAllocator>		m_pResCmdAlloc;
		ComPtr<ID3D12GraphicsCommandList>	m_pResCmdList;

		// Fence�� CPU/GPU ���ȭ�� ���� ����... (���߿��� ����ȭ ����)
		// https://docs.microsoft.com/ko-kr/windows/win32/direct3d12/user-mode-heap-synchronization
		ComPtr<ID3D12Fence>					m_pFence;
		uint32		m_fenceValue = 0;
		HANDLE		m_fenceEvent = INVALID_HANDLE_VALUE;

		shared_ptr<SwapChain>				m_pSwapChain;

	};
}
