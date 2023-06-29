#pragma once

#include "Graphics/Bindable/Bindable.h"

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
		struct TransformInfo
		{
			DirectX::XMFLOAT3 Position{ 0.0f, 0.0f, 0.0f };
			DirectX::XMFLOAT3 Rotation{ 0.0f, 0.0f, 0.0f };
			DirectX::XMFLOAT3 Scaling{ 1.0f, 1.0f, 1.0f };
		};
		TransformInfo Transform{};
	public:
		KDMesh(const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices,
			const std::string& meshName = "Unnamed Mesh");
		KDMesh(const KDMesh& mesh);
		~KDMesh() = default;

		KDMesh& operator = (const KDMesh& mesh);

		void Bind(KDGraphics& gfx);
		void SetNormalsFlat();
		void Clear();

		DirectX::XMMATRIX TransformMat() const;
	private:
		std::vector<std::shared_ptr<Bindable>> m_Binds;
	public:
		std::string Name;
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
	};
}