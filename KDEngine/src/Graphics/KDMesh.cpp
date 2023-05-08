#include "KDMesh.h"

#include <cassert>

namespace KDE
{
	KDMesh::KDMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		assert("Indices must be divide by 3." && indices.size() % 3 == 0);
		m_Vertices = vertices;
		m_Indices = indices;
	}
	KDMesh::KDMesh(const KDMesh& mesh)
	{
		operator=(mesh);
	}

	KDMesh& KDMesh::operator=(const KDMesh& mesh)
	{
		m_Vertices = mesh.m_Vertices;
		m_Indices = mesh.m_Indices;
	}

	const std::vector<Vertex>& KDMesh::Vertices() const
	{
		return m_Vertices;
	}
	const std::vector<uint32_t>& KDMesh::Indices() const
	{
		return m_Indices;
	}
	uint32_t KDMesh::VerticesCount() const
	{
		return m_Vertices.size();
	}
	uint32_t KDMesh::IndicesCount() const
	{
		return m_Indices.size();
	}
}