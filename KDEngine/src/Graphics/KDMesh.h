#pragma once

#include <vector>

namespace KDE
{
	struct Vertex
	{
		struct
		{
			float x, y, z;
		} pos;
	};

	class KDMesh
	{
	public:
		KDMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		KDMesh(const KDMesh& mesh);
		~KDMesh() = default;

		KDMesh& operator = (const KDMesh& mesh);

		const std::vector<Vertex>& Vertices() const;
		const std::vector<uint32_t>& Indices() const;

		uint32_t VerticesCount() const;
		uint32_t IndicesCount() const;
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}