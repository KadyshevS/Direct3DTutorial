#pragma once

#include <DirectXMath.h>

#include <vector>
#include <memory>

namespace KDE
{
	class VertexBuffer;
	class IndexBuffer;
	class Cube;

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
	};

	class KDMesh
	{
	public:
		KDMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		KDMesh(const KDMesh& mesh) = default;
		~KDMesh() = default;

		KDMesh& operator = (const KDMesh& mesh) = default;

		const std::vector<Vertex>& Vertices() const;
		const std::vector<uint32_t>& Indices() const;
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}