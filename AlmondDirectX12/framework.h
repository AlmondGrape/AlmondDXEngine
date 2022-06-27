#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C++ STL
#include <iostream>
#include <string>
#include <map>
#include <vector>

// Vector 등 수학관련함수.
#include "SimpleMath.h"

// DirectX 12 도우미 헤더 (https://github.com/microsoft/DirectX-Headers/blob/main/include/directx/d3dx12.h)
// MIT 라이센스 라서 WINDOWS D12X 기본 헤더에 포함 안되어 있음.. CD 계열 구조체 사용을 위해서...
// "무조건 d3d12.h 보다 먼저 include되어야 함!"
#include "Directx12Helper/d3dx12.h"

// DirectX 12
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

// 각종 lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

// 각종 using typedef
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// ENUM
enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0 = static_cast<uint8>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,

	END
};

enum 
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT,
};

// 임시
struct Transform
{
	DirectX::SimpleMath::Vector4 offset;
};

// 인스턴스 접근 & 에셋 불러오는 Define들
#define DEVICE Graphics_DX12::GetDX12()->GetDeviceInst()->GetDevice()
#define CMD_LIST	Graphics_DX12::GetDX12()->GetCmdQueueInst()->GetCmdList()
#define ROOT_SIGNATURE Graphics_DX12::GetDX12()->GetRootSignatureInst()->GetSignature()
#define TABLE_DESC_HEAP Graphics_DX12::GetDX12()->GetTableDescHeapInst()
#define DEPTHSTENCIL_BUFFER Graphics_DX12::GetDX12()->GetDepthStencilBuffer()
#define CONSTANT_BUFFER Graphics_DX12::GetDX12()->GetConstantBuffer