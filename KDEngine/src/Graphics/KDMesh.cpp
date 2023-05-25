#include "KDMesh.h"

#include "Bindable/VertexBuffer.h"
#include "Bindable/IndexBuffer.h"
#include "GeoPrimitives.h"

#include <cassert>

namespace KDE
{
	KDMesh::KDMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		assert("Indices must be divide by 3." && indices.size() % 3 == 0);
		m_Vertices = vertices;
		m_Indices = indices;
	}

	void KDMesh::Draw(KDGraphics& gfx)
	{

	}

	void KDMesh::SetNormalsFlat()
	{
		using namespace DirectX;
		assert(m_Indices.size() % 3 == 0 && m_Indices.size() > 0);
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			auto& v0 = m_Vertices[m_Indices[i]];
			auto& v1 = m_Vertices[m_Indices[i + 1]];
			auto& v2 = m_Vertices[m_Indices[i + 2]];
			const auto p0 = XMLoadFloat3(&v0.pos);
			const auto p1 = XMLoadFloat3(&v1.pos);
			const auto p2 = XMLoadFloat3(&v2.pos);

			const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

			XMStoreFloat3(&v0.norm, n);
			XMStoreFloat3(&v1.norm, n);
			XMStoreFloat3(&v2.norm, n);
		}
	}

	const std::vector<Vertex>& KDMesh::Vertices() const
	{
		return m_Vertices;
	}
	const std::vector<uint32_t>& KDMesh::Indices() const
	{
		return m_Indices;
	}
}