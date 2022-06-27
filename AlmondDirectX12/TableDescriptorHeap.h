#pragma once

using namespace Microsoft::WRL;
using namespace std;

namespace Almond
{
	class TableDescriptorHeap
	{
	public:
		void Initialize(uint32 count);
		void Clear();
		void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
		void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);
		void CommitTable();
		void Release();

		inline ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_pDescHeap; }

		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER reg);

	private:
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 reg);

	private:
		ComPtr<ID3D12DescriptorHeap>	m_pDescHeap;
		uint64							m_handleSize = 0;
		uint64							m_groupSize = 0;
		uint64							m_groupCount = 0;
		uint32							m_currentGroupIndex = 0;


	};
}
