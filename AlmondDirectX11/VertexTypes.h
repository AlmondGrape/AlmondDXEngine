#pragma once
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

namespace Almond
{

	struct Vertex
	{
		Vertex(Vector3 pos = Vector3(0.0f, 0.0f, 0.0f),
			Vector2 uv = Vector2(0.0f, 1.0f),
			Vector3 normal = Vector3(1.0f, 0.0f, 0.0f),
			Vector3 tangent = Vector3(0.0f, 0.0f, 0.0f),
			Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)) :
			Pos(pos), Uv(uv), Normal(normal), Tangent(tangent), Color(color) {}

		Vector3 Pos;
		Vector2 Uv;			// 2D �ؽ�ó �̹����� 3���� ������ �����￡ ������ ���� ��ȯ ������ �Ǵ� 2���� ��ǥ
		Vector3 Normal;		// ���� ����
		Vector3 Tangent;	// normal�� ������ ��鿡 �����ϴ� ���� (���������� �ִ� ����)
		Vector4 Color;

	};

	struct SkinnedVertex : public Vertex
	{
		SkinnedVertex(const Vertex& _vertex)
		{
			this->Pos = _vertex.Pos;
			this->Uv = _vertex.Uv;
			this->Normal = _vertex.Normal;
			this->Tangent = _vertex.Tangent;
			this->Color = _vertex.Color;

			Weight[0] = 0.0f;
			Weight[1] = 0.0f;
			Weight[2] = 0.0f;
			Weight[3] = 0.0f;

			BoneIndex[0] = 0;
			BoneIndex[1] = 0;
			BoneIndex[2] = 0;
			BoneIndex[3] = 0;
		};

		float Weight[4];
		int BoneIndex[4];
	};

}