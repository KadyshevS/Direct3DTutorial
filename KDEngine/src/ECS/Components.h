#pragma once

#include <Graphics/Bindable/TransformCBuffer.h>
#include "Graphics/KDGraphics.h"
#include "Graphics/KDMesh.h"
#include "Graphics/KDMath.h"

#include <string>
#include <memory>

namespace KDE::CS
{
	struct TagComponent
	{
		std::string Tag = "Unnamed Tag";

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent& operator = (const TagComponent&) = default;

		TagComponent(const std::string& tag)
		: Tag(tag) {}
	};

	struct RenderComponent
	{
		std::unique_ptr<KDMesh> Mesh;

		RenderComponent() = default;
		RenderComponent(const RenderComponent&) = delete;
		RenderComponent& operator = (const RenderComponent&) = delete;

		void Bind(KDGraphics& gfx)
		{
			Mesh->Bind(gfx);
		}
	};

	struct PointLightComponent
	{
	private:
		struct LightCBuffer
		{
			DirectX::XMFLOAT3 pos;
			float padding;
		};
		LightCBuffer lightCbuf{};
		std::unique_ptr<PixelConstantBuffer<LightCBuffer>> LightCBufferStr;

	public:
		PointLightComponent() = default;
		PointLightComponent(const PointLightComponent&) = delete;
		PointLightComponent& operator = (const PointLightComponent&) = delete;

		void Bind(KDGraphics& gfx, const DirectX::XMFLOAT3& data)
		{
			if (!LightCBufferStr)
			{
				LightCBufferStr = std::make_unique<PixelConstantBuffer<LightCBuffer>>(gfx, lightCbuf);
			}

			lightCbuf.pos = { data.x, data.y, data.z };

			LightCBufferStr->Update(gfx, lightCbuf);
			LightCBufferStr->Bind(gfx);
		}
	};
}