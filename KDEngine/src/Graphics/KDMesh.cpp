#include "KDMesh.h"

#include "Bindable/VertexBuffer.h"
#include "Bindable/IndexBuffer.h"
#include "Bindable/TransformCBuffer.h"
#include "GeoPrimitives.h"
#include "Graphics/KDMath.h"

#include <cassert>

namespace KDE
{
	KDMesh::KDMesh(const std::vector<Vertex>& vertices, 
		const std::vector<uint32_t>& indices)
	{
		assert("Indices must be divide by 3." && indices.size() % 3 == 0);
		m_Vertices = vertices;
		m_Indices = indices;
		SetNormalsFlat();
	}
	KDMesh::KDMesh(const KDMesh& mesh)
	{
		operator=(mesh);
	}

	KDMesh& KDMesh::operator=(const KDMesh& mesh)
	{
		assert("Indices must be divide by 3." && mesh.m_Indices.size() % 3 == 0);

		Clear();
		m_Vertices = mesh.m_Vertices;
		m_Indices = mesh.m_Indices;
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
			m_Binds.emplace_back(std::make_shared<KDE::VertexBuffer>(gfx, m_Vertices));
			m_Binds.emplace_back(std::make_shared<KDE::IndexBuffer>(gfx, m_Indices));
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
	void KDMesh::Clear()
	{
		if (!m_Vertices.empty())
			m_Vertices.clear();
		if (!m_Indices.empty())
			m_Indices.clear();
		if (!m_Binds.empty())
			m_Binds.clear();
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