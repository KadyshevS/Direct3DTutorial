#pragma once

#include "Bindable/TransformCBuffer.h"

#include <DirectXMath.h>
#include <vector>
#include <memory>

namespace KDE
{
	class KDGraphics;
	class VertexBuffer;
	class IndexBuffer;
	class Cube;

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT3 norm;
	};

	class KDMesh
	{
	public:
		struct
		{
			struct
			{
				float X = 0.0f, Y = 0.0f, Z = 0.0f;
			} Position;
			struct
			{
				float X = 0.0f, Y = 0.0f, Z = 0.0f;
			} Rotation;
			struct
			{
				float X = 1.0f, Y = 1.0f, Z = 1.0f;
			} Scaling;
		} Transform;
	public:
		KDMesh(KDGraphics& gfx,
			const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices);
		KDMesh(const KDMesh& mesh) = default;
		~KDMesh() = default;

		KDMesh& operator = (const KDMesh& mesh) = default;

		void Bind(KDGraphics& gfx);

		DirectX::XMMATRIX TransformMat() const;

		void SetNormalsFlat();

		const std::vector<Vertex>& Vertices() const;
		const std::vector<uint32_t>& Indices() const;
	private:
		std::vector<Bindable> m_Binds;
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		std::shared_ptr<TransformCBuffer> m_TransformCBuffer;
	};
}