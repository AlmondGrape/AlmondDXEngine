#pragma once

using namespace Microsoft::WRL;

namespace Almond
{
	class RasterizerState
	{
	public:
		RasterizerState() = default;
		~RasterizerState() = default;
		RasterizerState(const RasterizerState & other) = default;
		RasterizerState& operator =(const RasterizerState & other) = default;
		RasterizerState(RasterizerState && other) = default;

	public:
		void Initialize(ComPtr<ID3D11Device> device);
		void Release();

		void SetSolidState(ComPtr<ID3D11DeviceContext> context);
		void SetWireframeState(ComPtr<ID3D11DeviceContext> context);

	private:
		ComPtr<ID3D11RasterizerState> m_solidState;
		ComPtr<ID3D11RasterizerState> m_wireframeState;
		
	};
}
