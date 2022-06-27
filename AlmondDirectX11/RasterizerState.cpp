#include "pch.h"
#include "RasterizerState.h"

using namespace Almond;

void RasterizerState::Initialize(ComPtr<ID3D11Device> device)
{
	D3D11_RASTERIZER_DESC solidDesc;

	// Desc의 다른 원소들의 값은 기본값이 0일 때 보통의 기능을 냅니다. 따라서 Zeromemory로 초기화하고 실시합니다.
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;
	solidDesc.AntialiasedLineEnable = true;
	solidDesc.MultisampleEnable = true;
	solidDesc.DepthBias = 0;
	solidDesc.DepthBiasClamp = 0.0f;
	solidDesc.DepthClipEnable = true;
	solidDesc.ScissorEnable = false;
	solidDesc.SlopeScaledDepthBias = 0.0f;

	HRESULT hr = device->CreateRasterizerState(&solidDesc, m_solidState.GetAddressOf());

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;
	wireframeDesc.AntialiasedLineEnable = true;
	wireframeDesc.MultisampleEnable = true;
	wireframeDesc.DepthBias = 0;
	wireframeDesc.DepthBiasClamp = 0.0f;
	wireframeDesc.DepthClipEnable = true;
	wireframeDesc.ScissorEnable = false;
	wireframeDesc.SlopeScaledDepthBias = 0.0f;

	hr = device->CreateRasterizerState(&wireframeDesc, m_wireframeState.GetAddressOf());

}

void RasterizerState::Release()
{
	m_solidState.ReleaseAndGetAddressOf();
	m_wireframeState.ReleaseAndGetAddressOf();
}

void RasterizerState::SetSolidState(ComPtr<ID3D11DeviceContext> context)
{
	context->RSSetState(m_solidState.Get());
}

void RasterizerState::SetWireframeState(ComPtr<ID3D11DeviceContext> context)
{
	context->RSSetState(m_wireframeState.Get());
}
