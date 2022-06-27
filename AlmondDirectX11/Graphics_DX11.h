#pragma once
#include "IGraphics.h"
#include "Device.h"
#include "RasterizerState.h"
#include "SwapChain.h"
#include "DepthStencilState.h"

namespace Almond
{
	// D11 ���� ����ϴ� �޴� ������ ���� ����..
	struct WindowInfo
	{
		HWND	hWnd;
		int		width;
		int		height;
		bool	windowed;

		bool	enable4xMSAA;
		uint32	MSAAQuality;
	};

	class Graphics_DX11 : public IGraphics
	{
	public:

		virtual void Initialize(HWND hWnd, int width, int height) override;

		virtual void BeginRender() override;
		virtual void Render() override;
		virtual void EndRender() override;

		virtual void Release() override;

		void ResizeWindow(int32 width, int32 height);

	private:
		static shared_ptr<Graphics_DX11> GetDX11() { return s_pGraphics_DX11; }


	private:		
		WindowInfo						m_windowInfo;
		
		shared_ptr<Device>				m_pDevice;
		shared_ptr<RasterizerState>		m_pRasterizerState;
		shared_ptr<SwapChain>			m_pSwapChain;
		shared_ptr<DepthStencilState>	m_pDepthStencilState;

		// ���� Ÿ�ٿ� �׸� �� ����� ����Ʈ
		D3D11_VIEWPORT					m_viewPort = {};


		// ���� ���ο����� �ڵ尡 ���ư� �� ����ϴ� ĳ���õ� ������
		static shared_ptr<Graphics_DX11> s_pGraphics_DX11;

	};

}

