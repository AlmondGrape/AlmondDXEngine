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
		Vector2 Uv;			// 2D 텍스처 이미지를 3차원 공간의 폴리곤에 입히기 위한 변환 기준이 되는 2차원 좌표
		Vector3 Normal;		// 법선 벡터
		Vector3 Tangent;	// normal에 수직인 평면에 존재하는 벡터 (접선공간에 있는 벡터)
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