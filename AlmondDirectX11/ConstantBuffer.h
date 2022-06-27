#pragma once
#include "Shader_def.h"

namespace Almond
{	
	// 상수 타입을 템플릿으로 받아서 버퍼로 초기화하여 관리..
	template<class T>
	class ConstantBuffer
	{
	public:
		ConstantBuffer() {}		

		ID3D11Buffer* Get()const
		{
			return buffer.Get();
		}

		ID3D11Buffer* const* GetAddressOf()const
		{
			return buffer.GetAddressOf();
		}

		HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, CBV_REGISTER slot)
		{
			if (buffer.Get() != nullptr)
				buffer.Reset();

			m_slot = slot;

			this->deviceContext = deviceContext;

			D3D11_BUFFER_DESC desc;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = NULL;
			desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
			desc.StructureByteStride = NULL;

			HRESULT hr = device->CreateBuffer(&desc, 0, buffer.GetAddressOf());
			return hr;
		}

		void Release()
		{
			m_pConstantBuffer.ReleaseAndGetAddressOf();
			m_pContext = nullptr;
		}

		bool ApplyChanges()
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{				
				return false;
			}
			CopyMemory(mappedResource.pData, &m_constantData, sizeof(T));
			this->deviceContext->Unmap(buffer.Get(), 0);
			return true;
		}

	private:
		ConstantBuffer(const ConstantBuffer<T>& rhs);

	public:
		T m_constantData;

		CBV_REGISTER m_slot;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
		ID3D11DeviceContext* m_pContext = nullptr;
	};
}
