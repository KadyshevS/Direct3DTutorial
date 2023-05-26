#pragma once

#include "Graphics/KDMath.h"

namespace KDE
{
	class KDCamera
	{
	private:
		struct TransformInfo
		{
			struct
			{
				float X = 0.0f, Y = 0.0f, Z = 0.0f;
			} Position;
			struct
			{
				float X = 0.0f, Y = 0.0f, Z = 0.0f;
			} Rotation;
		};
		struct ProjectionInfo
		{
			float Width = 800, Height = 600;
			float Near = 0.5f, Far = 40.0f;
		};
	public:
		TransformInfo Transform{};
		ProjectionInfo Projection{};

	public:
		KDCamera() = default;

		DirectX::XMMATRIX ProjectionMat() const
		{
			return
				DirectX::XMMatrixPerspectiveLH(
					1.0f, Projection.Height / Projection.Width, 
					Projection.Near, Projection.Far);
		}
		DirectX::XMMATRIX ViewMat() const
		{
			return
				DirectX::XMMatrixInverse(nullptr,
				DirectX::XMMatrixRotationRollPitchYaw(
					Math::ToRadians(Transform.Rotation.X),
					Math::ToRadians(Transform.Rotation.Y),
					Math::ToRadians(Transform.Rotation.Z)) *
				DirectX::XMMatrixTranslation(
					Transform.Position.X,
					Transform.Position.Y,
					Transform.Position.Z));
		}
	};
}