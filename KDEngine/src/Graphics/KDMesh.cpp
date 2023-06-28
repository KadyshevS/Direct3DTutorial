#include "Graphics/KDMesh.h"

#include "Graphics/Bindable/VertexBuffer.h"
#include "Graphics/Bindable/IndexBuffer.h"
#include "Graphics/Bindable/TransformCBuffer.h"
#include "Graphics/GeoPrimitives.h"
#include "Graphics/KDMath.h"

#include <cassert>

namespace KDE
{
	KDMesh::KDMesh(const std::vector<Vertex>& vertices, 
		const std::vector<uint32_t>& indices,
		const std::string& meshName)
		: Name(meshName)
	{
		assert("Indices must be divide by 3." && indices.size() % 3 == 0);
		Vertices = vertices;
		Indices = indices;
		SetNormalsFlat();
	}
	KDMesh::KDMesh(const KDMesh& mesh)
	{
		operator=(mesh);
	}

	KDMesh& KDMesh::operator=(const KDMesh& mesh)
	{
		assert("Indices must be divide by 3." && mesh.Indices.size() % 3 == 0);

		Clear();
		Vertices = mesh.Vertices;
		Indices  = mesh.Indices;
		Name     = mesh.Name;
		m_Binds.resize(m_Binds.size());
		for (int i = 0; i < mesh.m_Binds.size(); i++)
		{
			m_Binds[i] = mesh.m_Binds[i];
		}
		SetNormalsFlat();

		return *this;
	}

	void KDMesh::Bind(KDGraphics& gfx)
	{
		if (m_Binds.empty())
		{
			m_Binds.emplace_back(std::make_shared<KDE::VertexBuffer>(gfx, Vertices));
			m_Binds.emplace_back(std::make_shared<KDE::IndexBuffer>(gfx, Indices));
			m_Binds.emplace_back(std::make_shared<KDE::TransformCBuffer>(gfx, *this));
		}

		for (auto& b : m_Binds)
		{
			b->Bind(gfx);
		}
	}

	DirectX::XMMATRIX KDMesh::TransformMat() const
	{
		return
			DirectX::XMMatrixRotationRollPitchYaw(
				Math::ToRadians(Transform.Rotation.X),
				Math::ToRadians(Transform.Rotation.Y),
				Math::ToRadians(Transform.Rotation.Z)) *
			DirectX::XMMatrixScaling(
				Transform.Scaling.X,
				Transform.Scaling.Y,
				Transform.Scaling.Z) *
			DirectX::XMMatrixTranslation(
				Transform.Position.X,
				Transform.Position.Y,
				Transform.Position.Z);
	}

	void KDMesh::SetNormalsFlat()
	{
		using namespace DirectX;
		assert(Indices.size() % 3 == 0 && Indices.size() > 0);
		for (size_t i = 0; i < Indices.size(); i += 3)
		{
			auto& v0 = Vertices[Indices[i]];
			auto& v1 = Vertices[Indices[i + 1]];
			auto& v2 = Vertices[Indices[i + 2]];
			const auto p0 = XMLoadFloat3(&v0.pos);
			const auto p1 = XMLoadFloat3(&v1.pos);
			const auto p2 = XMLoadFloat3(&v2.pos);

			const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

			XMStoreFloat3(&v0.norm, n);
			XMStoreFloat3(&v1.norm, n);
			XMStoreFloat3(&v2.norm, n);
		}
	}
	void KDMesh::Clear()
	{
		if (!Vertices.empty())
			Vertices.clear();
		if (!Indices.empty())
			Indices.clear();
		if (!m_Binds.empty())
			m_Binds.clear();
	}
}