#pragma once

#include "Graphics/KDGraphics.h"

#include <DirectXMath.h>
#include <string>

namespace KDE
{
	struct TagComponent
	{
		std::string Name;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& name)
			: Name(name)
		{}
	};

	struct TransformComponent
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
		} Scale;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		
		void Bind(KDGraphics& gfx)
		{
			cb1.transform = DirectX::XMMatrixTranspose(Transform() * gfx.Projection());

			if (m_VertexConstantBuffer == nullptr)
				m_VertexConstantBuffer = std::make_shared<VertexConstantBuffer<ConstantBuffer1>>(gfx, cb1);

			m_VertexConstantBuffer->Update(gfx, cb1);
			m_VertexConstantBuffer->Bind(gfx);
		}

		DirectX::XMMATRIX Transform() const
		{
			namespace dx = DirectX;

			return dx::XMMatrixRotationRollPitchYaw(Rotation.X, Rotation.Y, Rotation.Z) * 
				dx::XMMatrixTranslation(Position.X, Position.Y, Position.Z) *
				dx::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z);
		}

	private:
		struct ConstantBuffer1
		{
			DirectX::XMMATRIX transform;
		};
		ConstantBuffer1 cb1{};
		std::shared_ptr<VertexConstantBuffer<ConstantBuffer1>> m_VertexConstantBuffer = nullptr;
	};

	struct MeshComponent
	{
		std::shared_ptr<KDMesh> Mesh = nullptr;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(KDMesh mesh)
			: Mesh(std::make_shared<KDMesh>(mesh))
		{}

		void Bind(KDGraphics& gfx)
		{
			if (m_VertexBuffer == nullptr)
				m_VertexBuffer = std::make_shared<VertexBuffer>(gfx, Mesh->Vertices());

			m_VertexBuffer->Bind(gfx);

			if (m_IndexBuffer == nullptr)
				m_IndexBuffer = std::make_shared<IndexBuffer>(gfx, Mesh->Indices());

			m_IndexBuffer->Bind(gfx);
		}

	private:
		std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
		std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	};
}