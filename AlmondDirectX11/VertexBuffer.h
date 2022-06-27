#pragma once
#include "VertexTypes.h"

namespace Almond
{
	/// <summary>
	/// 메쉬를 구성하는 버텍스 버퍼
	/// 2022.04.14 nut
	/// </summary>
	/// <typeparam name="T">보통 Vertex구조체</typeparam>
	template<class T>
	class VertexBuffer
	{
	public:
		VertexBuffer() {}

		// 버텍스데이터로 버퍼를 만드는 초기화함수.
		HRESULT Initialize(ID3D11Device* device, T* data, uint32 vertexCount)
		{
			if (m_pVertexBuffer.Get() != nullptr)
			{
				m_pVertexBuffer.Reset();
			}

			this->m_vertexCount = vertexCount;

			// vertex buffer를 만들고
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

			// 버텍스들의 데이터를 넣어주고...
			_vinitData.pSysMem = data;

			// 버텍스 버퍼 생성.
			HRESULT _hr = device->CreateBuffer(&_vertexBufferDesc, &_vinitData, &m_pVertexBuffer);

			return _hr;
		}

		void Release()
		{
			m_pVertexBuffer.ReleaseAndGetAddressOf();
			m_sizeOfInstance = 0;
			m_vertexCount = 0;
		}

		// 멤버변수 접근함수
		ID3D11Buffer* Get() const { return m_pVertexBuffer.Get(); }
		ID3D11Buffer* const* GetAddressOf() const { return m_pVertexBuffer.GetAddressOf(); }
		uint32 VertexCount() const { return this->m_vertexCount; }
		const uint32 SizeOfInstance() const { return this->m_sizeOfInstance; }
		const uint32* SizeOfInstancePtr() const { return &this->m_sizeOfInstance; }

		// 연산자 오버로딩
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