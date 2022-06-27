#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace Almond
{
	/// <summary>
	/// �޽��� �����ϴ� �ε��� ����
	/// 2022.04.14 nut
	/// </summary>
	class IndexBuffer
	{
	public:
		IndexBuffer() {}

		HRESULT Initialize(ID3D11Device* device, uint32* data, uint32 indexCount)
		{
			if (m_pIndexBuffer.Get() != nullptr)
			{
				m_pIndexBuffer.Reset();
			}
			this->m_indexCount = indexCount;

			// index ���۸� �����..
			D3D11_BUFFER_DESC _indexBufferDesc;
			ZeroMemory(&_indexBufferDesc, sizeof(_indexBufferDesc));

			_indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			_indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
			_indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			_indexBufferDesc.CPUAccessFlags = 0;
			_indexBufferDesc.MiscFlags = 0;
			_indexBufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA _iinitData;
			ZeroMemory(&_iinitData, sizeof(_iinitData));

			// �ε��� �����͸� �־��ְ�..
			_iinitData.pSysMem = data;

			HRESULT _hr = device->CreateBuffer(&_indexBufferDesc, &_iinitData, &m_pIndexBuffer);

			return _hr;

		}

		void Release()
		{
			m_pIndexBuffer.ReleaseAndGetAddressOf();
			m_indexCount = 0;
		}

		// ������� ���� �Լ�
		ID3D11Buffer* Get() const { return m_pIndexBuffer.Get(); }
		ID3D11Buffer* const* GetAddressOf() const { return m_pIndexBuffer.GetAddressOf(); }
		uint32 IndexCount() const { return this->m_indexCount; }


	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
		uint32 m_indexCount = 0;

	};
}