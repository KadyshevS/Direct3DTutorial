#include "KDMesh.h"

#include "Bindable/VertexBuffer.h"
#include "Bindable/IndexBuffer.h"
#include "Graphics/KDGraphics.h"
#include "GeoPrimitives.h"

#include <cassert>

namespace KDE
{
	KDMesh::KDMesh(KDGraphics& gfx, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		assert("Indices must be divide by 3." && indices.size() % 3 == 0);
		m_Vertices = vertices;
		m_Indices = indices;
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