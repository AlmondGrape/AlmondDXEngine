#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "Shader_def.h"
#include "ConstantBuffer.h"
#include "ConstantTypes.h"

using namespace Microsoft::WRL;

struct ShaderResource
{
	ComPtr<ID3D11ShaderResourceView> _srv;

	// TO DO: 나중에 텍스쳐 요소가 많아지면, 몇번 Slot인지 RTTI를 통해서 찾아주자.

	SRV_REGISTER _slot;
};

struct Sampler
{
	ComPtr<ID3D11SamplerState> _samplerState;

	// TO DO: 나중에 샘플러 요소가 많아지면, 몇번 Slot인지 RTTI를 통해서 찾아주자.

	SAM_REGISTER _slot;
};

namespace Almond
{
	template <SHADER_TYPE ShaderType>
	class IShader
	{
	public:
		virtual void Initialize(const wstring& filePath, const wstring& shaderName) abstract;
		virtual void Release() abstract;
		virtual void Update() abstract;



	private:		
		std::vector<std::shared_ptr<ConstantBuffer>>	m_pConstantBuffers;
		std::vector<std::shared_ptr<ShaderResource>>	m_pShaderResources;
		std::vector<std::shared_ptr<Sampler>>			m_pSamplerStates;
		
		SHADER_TYPE		m_pShaderType;
		std::wstring	m_shaderName;

	};
}