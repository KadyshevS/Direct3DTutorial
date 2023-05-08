#pragma once

#include <DirectXMath.h>
#include <string>

namespace KDE
{
	struct TagComponent
	{
		std::string Name;

		TagComponent() = default;
		TagComponent(const TagComponent&) = delete;
		TagComponent(const std::string& name)
			: Name(name)
		{}
	};

	struct TransformComponent
	{
		float PositionX = 0.0f, PositionY = 0.0f, PositionZ = 0.0f;
		float RotationX = 0.0f, RotationY = 0.0f, RotationZ = 0.0f;
		float ScaleX	= 1.0f, ScaleY	  = 1.0f, ScaleZ	= 1.0f;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = delete;
		
		DirectX::XMMATRIX Transform() const
		{
			namespace dx = DirectX;

			return dx::XMMatrixTranslation(PositionX, PositionY, PositionZ) *
				dx::XMMatrixRotationRollPitchYaw(RotationX, RotationY, RotationZ) *
				dx::XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
		}

		DirectX::XMVECTOR Position() const
		{
			return { PositionX, PositionY, PositionZ };
		}
		DirectX::XMVECTOR Rotation() const
		{
			return { RotationX, RotationY, RotationZ };
		}
		DirectX::XMVECTOR Scale() const
		{
			return { ScaleX, ScaleY, ScaleZ };
		}
	};
}