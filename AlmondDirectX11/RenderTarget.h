#pragma once
#include "SimpleMath.h"

using namespace Microsoft::WRL;

namespace Almond
{
	class RenderTarget
	{
	public:
		RenderTarget() = default;
		~RenderTarget() = default;
		RenderTarget(const RenderTarget& other) = default;
		RenderTarget& operator =(const RenderTarget& other) = default;
		RenderTarget(RenderTarget&& other) = default;

	public:
		void Initialize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight, bool onlyNeedDepthBuffer);
		void Release();

		void SetRenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void ClearRenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, DirectX::SimpleMath::Vector4 color = { 0.1f, 0.1f, 0.1f, 1.0f });

		void SetRTV(ComPtr<ID3D11RenderTargetView> rtv) { m_pRenderTargetView = rtv; }

	private:
		ComPtr<ID3D11Texture2D>				m_pRenderTargetTexture;
		ComPtr<ID3D11RenderTargetView>		m_pRenderTargetView;
		ComPtr<ID3D11ShaderResourceView>	m_pShaderResourceView;

		// ÀÌ ·»´õ Å¸°ÙÀÌ »ç¿ëÇÏ´Â µª½º½ºÅÙ½Ç ¹öÆÛ ÅØ½ºÃÄ ¹× µª½º ½ºÅÙ½Ç ºä
		ComPtr<ID3D11Texture2D>				m_pDepthStencilBuffer;
		ComPtr<ID3D11DepthStencilView>		m_pDepthStencilView;

	};
}
