#pragma once

using namespace Microsoft::WRL;
using namespace std;

namespace Almond
{
	enum class CONSTANT_BUFFER_TYPE : uint8
	{
		TRANSFORM,
		MATERIAL,
		END
	};

	enum
	{
		CONSTANT_BUFFER_COUNT = static_cast<uint8>(CONSTANT_BUFFER_TYPE::END)
	};

	// GPU �޸� ���� �� ConstantBuffer Resource�� �Ҵ�� �޸� ���� ��ü
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		~ConstantBuffer();
		ConstantBuffer(const ConstantBuffer& other) = default;
		ConstantBuffer& operator =(const ConstantBuffer& other) = default;
		ConstantBuffer(ConstantBuffer&& other) = default;

	public:
		void Initialize(CBV_REGISTER reg, uint32 size, uint32 count);
		void Clear();
		void PushData(void* buffer, uint32 size);
		void Release();

		D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

	private:
		void CreateBuffer();
		void CreateView();

	private:
		// ��� ���۵��� �ּ�.. �Ϸķ� ������
		ComPtr<ID3D12Resource>	m_pCbvBuffer;
		BYTE*					m_pMappedBuffer = nullptr;
		// ��� ������ ������
		uint32					m_elementSize = 0;
		uint32					m_elementCount = 0;

		ComPtr<ID3D12DescriptorHeap>	m_pCbvHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE		m_cpuHandleBegin = {};
		uint32							m_handleIncrementSize = 0;

		// �� �������� �� �׷����� ��, 0���� �ʱ�ȭ, �ε��� ����
		uint32							m_currentIndex = 0;

		CBV_REGISTER					m_reg = {};

	};
}

