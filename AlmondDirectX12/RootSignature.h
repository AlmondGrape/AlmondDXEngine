#pragma once

using namespace Microsoft::WRL;

namespace Almond
{
	// Root Signature
	// �׸��� ȣ�� ���� ���� ���α׷��� �ݵ�� ������ ���������ο� ������ �ϴ� �ڿ����� �����̰�,
	// �� �ڿ����� ���̴� �Է� �������͵鿡 ��� �����Ǵ����� ���� �ϴ� ��.
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
