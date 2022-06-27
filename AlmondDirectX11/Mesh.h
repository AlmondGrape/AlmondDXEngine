#pragma once
#include "VertexTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

using namespace Microsoft::WRL;

namespace Almond
{	
	class Mesh
	{
	public:
		Mesh() {}
		virtual ~Mesh() {}
		Mesh(const Mesh& other) = default;
		Mesh& operator =(const Mesh& other) = default;
		Mesh(Mesh&& other) = default;

	public:
		void Initialize(std::vector<Vertex>& vertexData, std::vector<uint32>& indexData);
		void Update();
		void Release();

	protected:
		std::shared_ptr<VertexBuffer<Vertex>> m_pVertexBuffer;
		std::shared_ptr<IndexBuffer>		  m_pIndexBuffer;
		D3D11_PRIMITIVE_TOPOLOGY			  m_topology = {};

		uint32 m_stride = 0;
		uint32 m_offset = 0;
		uint32 m_indexBufferSize = 0;

	};
}
