#pragma once

using namespace Microsoft::WRL;

namespace Almond
{
	// Root Signature
	// 그리기 호출 전에 응용 프로그램이 반드시 렌더링 파이프라인에 묶여야 하는 자원들이 무엇이고,
	// 그 자원들이 셰이더 입력 레지스터들에 어떻게 대응되는지를 정의 하는 것.
	class RootSignature
	{
	public:
		void Initialize(ComPtr<ID3D12Device> device);
		void Release();
		ComPtr<ID3D12RootSignature> GetSignature() { return m_pRootSignature; }

	private:
		void CreateSamplerDesc();
		void CreateRootSignature(ComPtr<ID3D12Device> device);

	private:

		ComPtr<ID3D12RootSignature> m_pRootSignature;
		D3D12_STATIC_SAMPLER_DESC	m_samplerDesc;

	};
}
