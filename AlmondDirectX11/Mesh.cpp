#include "pch.h"
#include "Mesh.h"


void Almond::Mesh::Initialize(std::vector<Vertex>& vertexData, std::vector<uint32>& indexData)
{
	m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pVertexBuffer = std::make_shared<VertexBuffer<Vertex>>();
	m_pIndexBuffer = std::make_shared<IndexBuffer>();




}

void Almond::Mesh::Update()
{

}

void Almond::Mesh::Release()
{
	

}
