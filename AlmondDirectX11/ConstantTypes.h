#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

struct ConstantBufferBase
{
	// RTTI�� ����ϱ� ���� �����Լ� ���� �غ���.
	virtual ~ConstantBufferBase()
	{

	}
};

// 16����Ʈ ����. (XMMATRIX ���� �� ��������� ���� ������ �ʿ���. - ������ �߰� �ǰ���.)
__declspec(align(16)) struct CB_TEST : public ConstantBufferBase
{
	Vector4 _offset;
};