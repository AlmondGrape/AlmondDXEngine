#include "pch.h"
#include "pch.h"
#include "RenderTarget.h"

using namespace Almond;

void RenderTarget::Initialize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight, bool onlyNeedDepthBuffer)
{
	// ���� ���� �����	
	D3D11_TEXTURE2D_DESC _depthStencilDesc;
	ZeroMemory(&_depthStencilDesc, sizeof(_depthStencilDesc));

	_depthStencilDesc.Width = textureWidth;
	_depthStencilDesc.Height = textureHeight;
	_depthStencilDesc.MipLevels = 1;
	_depthStencilDesc.ArraySize = 1;

	// ���ٽ� ��� ���� ��� ����.
	_depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 4X MSAA ���..?
	_depthStencilDesc.SampleDesc.Count = 4;
	_depthStencilDesc.SampleDesc.Quality = 0;

	_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	_depthStencilDesc.CPUAccessFlags = 0;
	_depthStencilDesc.MiscFlags = 0;

	// Depth Buffer ����
	HR(device->CreateTexture2D(&_depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf()));

	// ���� ���� �� �����
	// DepthStencilView�� ���� �� ����� ���� ���ٽ� �� ����ũ���̺긦 �ۼ��ϰ� �Ű������� ������.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(device->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf()));

	if (onlyNeedDepthBuffer)
		return;

	// ���̴� ���ҽ� �� �ʱ�ȭ

}

void RenderTarget::SetRenderTarget(ComPtr<ID3D11DeviceContext> context)
{
	// ���� Ÿ�� ��� ���� ���ٽ� ���۸� ���������ο� ���ε�
	context->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
}

void RenderTarget::ClearRenderTarget(ComPtr<ID3D11DeviceContext> context, DirectX::SimpleMath::Vector4 color)
{
	float _color[4];
	_color[0] = color.x;
	_color[1] = color.y;
	_color[2] = color.z;
	_color[3] = color.w;

	context->ClearRenderTargetView(m_pRenderTargetView.Get(), _color);
	context->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

void Almond::RenderTarget::Release()
{
	m_pRenderTargetTexture.ReleaseAndGetAddressOf();
	m_pRenderTargetView.ReleaseAndGetAddressOf();
	m_pShaderResourceView.ReleaseAndGetAddressOf();
	m_pDepthStencilBuffer.ReleaseAndGetAddressOf();
	m_pDepthStencilView.ReleaseAndGetAddressOf();
}
