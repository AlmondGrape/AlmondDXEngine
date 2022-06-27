#pragma once
#include "VertexTypes.h"

namespace Almond
{
	/// <summary>
	/// �޽��� �����ϴ� ���ؽ� ����
	/// 2022.04.14 nut
	/// </summary>
	/// <typeparam name="T">���� Vertex����ü</typeparam>
	template<class T>
	class VertexBuffer
	{
	public:
		VertexBuffer() {}

		// ���ؽ������ͷ� ���۸� ����� �ʱ�ȭ�Լ�.
		HRESULT Initialize(ID3D11Device* device, T* data, uint32 vertexCount)
		{
			if (m_pVertexBuffer.Get() != nullptr)
			{
				m_pVertexBuffer.Reset();
			}

			this->m_vertexCount = vertexCount;

			// vertex buffer�� �����
			D3D11_BUFFER_DESC _vertexBufferDesc;
			ZeroMemory(&_vertexBufferDesc, sizeof(_vertexBufferDesc));

			_vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			_vertexBufferDesc.CPUAccessFlags = 0;

			_vertexBufferDesc.ByteWidth = m_sizeOfInstance * m_vertexCount;
			_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			
			_vertexBufferDesc.MiscFlags = 0;
			_vertexBufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA _vinitData;
			ZeroMemory(&_vinitData, sizeof(_vinitData));

			// ���ؽ����� �����͸� �־��ְ�...
			_vinitData.pSysMem = data;

			// ���ؽ� ���� ����.
			HRESULT _hr = device->CreateBuffer(&_vertexBufferDesc, &_vinitData, &m_pVertexBuffer);

			return _hr;
		}

		void Release()
		{
			m_pVertexBuffer.ReleaseAndGetAddressOf();
			m_sizeOfInstance = 0;
			m_vertexCount = 0;
		}

		// ������� �����Լ�
		ID3D11Buffer* Get() const { return m_pVertexBuffer.Get(); }
		ID3D11Buffer* const* GetAddressOf() const { return m_pVertexBuffer.GetAddressOf(); }
		uint32 VertexCount() const { return this->m_vertexCount; }
		const uint32 SizeOfInstance() const { return this->m_sizeOfInstance; }
		const uint32* SizeOfInstancePtr() const { return &this->m_sizeOfInstance; }

		// ������ �����ε�
		VertexBuffer(const VertexBuffer<T>& target)
		{
			this->m_pVertexBuffer = target.m_pVertexBuffer;
			this->m_vertexCount = target.m_vertexCount;
			this->m_sizeOfInstance = target.m_sizeOfInstance;
		}

		VertexBuffer<T>& operator = (const VertexBuffer<T>& target)
		{
			this->m_pVertexBuffer = target.m_pVertexBuffer;
			this->m_vertexCount = target.m_vertexCount;
			this->m_sizeOfInstance = target.m_sizeOfInstance;

			return *this;
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
		uint32 m_sizeOfInstance = sizeof(T);
		uint32 m_vertexCount = 0;
	};
}