#include "pch.h"
#include "DepthStencilState.h"
#include "Device.h"

using namespace Almond;

void DepthStencilState::Initialize(std::shared_ptr<Device> device)
{
	ComPtr<ID3D11Device> _device = device->GetDevice();
	m_pD3DDeviceContext = device->GetDeviceContext();

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	// Stencil operations if pixel is front-facing. 
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing. 
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	_device->CreateDepthStencilState(&depthStencilDesc, m_pDepthStencilState.GetAddressOf());

	// Z 버퍼링이 꺼진 뎁스 스텐실 스테이트를 생성
	D3D11_DEPTH_STENCIL_DESC depthStencilOffDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilOffDesc.DepthEnable = false;
	depthStencilOffDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilOffDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilOffDesc.StencilEnable = true;
	depthStencilOffDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilOffDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	// Stencil operations if pixel is front-facing. 
	depthStencilOffDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilOffDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilOffDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilOffDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing. 
	depthStencilOffDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilOffDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilOffDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilOffDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	_device->CreateDepthStencilState(&depthStencilOffDesc, m_pDepthStencilStateZOff.GetAddressOf());

}

void Almond::DepthStencilState::Release()
{
	m_pDepthStencilState.ReleaseAndGetAddressOf();
	m_pDepthStencilStateZOff.ReleaseAndGetAddressOf();	
}

void Almond::DepthStencilState::SetDepthEnableState()
{
	m_pD3DDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), NULL);
}

void Almond::DepthStencilState::SetDepthDisableState()
{
	m_pD3DDeviceContext->OMSetDepthStencilState(m_pDepthStencilStateZOff.Get(), NULL);
}
