#pragma once
#include "IGraphics.h"
#include "Device.h"
#include "SwapChain.h"
#include "CommandQueue.h"
#include "RootSignature.h"
#include "TableDescriptorHeap.h"
#include "DepthStencilBuffer.h"
#include "ConstantBuffer.h"

namespace Almond
{
	// D12 에서 사용하는 받는 윈도우 변수 모음..
	struct WindowInfo
	{
		HWND	hWnd;
		int		width;
		int		height;
		bool	windowed;
	};

	class Graphics_DX12 : public IGraphics
	{
	public:
		virtual void Initialize(HWND hWnd, int width, int height) override;

		virtual void BeginRender() override;
		virtual void Render() override;
		virtual void EndRender() override;

		virtual void Release() override;

		void ResizeWindow(int32 width, int32 height);


		// 멤버함수 GetSet
	public:
		static shared_ptr<Graphics_DX12> GetDX12() { return s_pGraphics_DX12; }

		shared_ptr<RootSignature> GetRootSignatureInst() { return m_pRootSignature; }
		shared_ptr<Device> GetDeviceInst() { return m_pDevice; }
		shared_ptr<CommandQueue> GetCmdQueueInst() { return m_pCmdQueue; }
		shared_ptr<TableDescriptorHeap> GetTableDescHeapInst() { return m_pTableDescriptorHeap; }
		shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer() { return m_pDepthStencilBuffer; }
		shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return m_pConstantBuffers[static_cast<uint8>(type)]; }

	private:
		void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);


	private:
		// 그려질 화면 정보
		WindowInfo		m_windowInfo;
		D3D12_VIEWPORT	m_viewPort = {};
		D3D12_RECT		m_scissorRect = {};

		shared_ptr<Device>				m_pDevice;
		shared_ptr<CommandQueue>		m_pCmdQueue;
		shared_ptr<SwapChain>			m_pSwapChain;
		shared_ptr<RootSignature>		m_pRootSignature;
		shared_ptr<TableDescriptorHeap>	m_pTableDescriptorHeap;
		shared_ptr<DepthStencilBuffer>	m_pDepthStencilBuffer;

		vector<shared_ptr<ConstantBuffer>> m_pConstantBuffers;

		// 엔진 내부에서의 코드가 돌아갈 때 사용하는 캐스팅된 포인터
		static shared_ptr<Graphics_DX12> s_pGraphics_DX12;

	};
}


/// D12 초기화 순서
/// 파이프라인 초기화.
// 1. Enable the debug layer.										 (Device.h)
// 2. Create the device.											 (Device.h)
// 3. Create the swap chain.										 (SwapChain.h)
// 4. Create a render target view(RTV) descriptor heap.				 (SwapChain.h)
//   -  A descriptor heap can be thought of as an array of descriptors.
//      Where each descriptor fully describes an object to the GPU.
// 5. Create frame resources(a render target view for each frame).	 (SwapChain.h)
// 6. Create the command queue.										 (CommandQueue.h)
// 7. Create a command allocator.									 (CommandQueue.h)
//   -  A command allocator manages the underlying storage for command listsand bundles.

/// 에셋 초기화.
// A1. Create an empty root signature.			(RootSignature.h)
// A2. A graphics root signature defines what resources are bound to the graphics pipeline.
// A3. Compile the shaders.
// A4. Create the vertex input layout.
// A5. Create a pipeline state object description, then create the object.
//   -  A pipeline state object maintains the state of all currently set shaders as well as certain fixed function state objects(such as the input assembler, tesselator, rasterizer and output merger).
// A6. Create the command list.		(CommandQueue.h)
// A7. Close the command list.	 	(CommandQueue.h)
// A8. Create and load the vertex buffers.
// A9. Create the vertex buffer views.
// A10. Create a fence.				 (CommandQueue.h)
//   -  A fence is used to synchronize the CPU with the GPU(see Multi - engine synchronization).
// A11. Create an event handle.		 (CommandQueue.h)
// A12. Wait for the GPU to finish.  (CommandQueue.h)
// A13. Check on the fence!			 (CommandQueue.h)

/// D12 랜더 순서 (CommandQueue.h / Begin & End Render)
// R1. Populate the command list.
//   r1. Reset the command list allocator.
//        - Re-use the memory that is associated with the command allocator.
//   r2. Reset the command list.
//   r3. Set the graphics root signature.
//        - Sets the graphics root signature to use for the current command list.
//   r4. Set the viewport and scissor rectangles.
//   r5. Set a resource barrier, indicating the back buffer is to be used as a render target.
//        - Resource barriers are used to manage resource transitions.
//   r6. Record commands into the command list.
//   r7. Indicate the back buffer will be used to present after the command list has executed.
//        - Another call to set a resource barrier.
//   r8. Close the command list to further recording.
// R2. Execute the command list.
// R3. Present the frame.
// R4. Wait for the GPU to finish