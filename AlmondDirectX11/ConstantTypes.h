#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

struct ConstantBufferBase
{
	// RTTI를 사용하기 위한 가상함수 따라 해보자.
	virtual ~ConstantBufferBase()
	{

	}
};

// 16바이트 정렬. (XMMATRIX 같은 걸 멤버변수로 갖고 있으면 필요함. - 언젠간 추가 되겠지.)
__declspec(align(16)) struct CB_TEST : public ConstantBufferBase
{
	Vector4 _offset;
};