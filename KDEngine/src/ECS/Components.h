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
	private:
		struct ObjectCBufferInfo
		{
			alignas(16) DirectX::XMFLOAT3 MaterialColor;
			float SpecularIntensity = 0.6f;
			float SpecularPower = 30.0f;
			float padding[2];
		};
		std::unique_ptr<PixelConstantBuffer<ObjectCBufferInfo>> ObjectCBuf;
	public:
		ObjectCBufferInfo ObjectCBuffer{};
	public:
		std::unique_ptr<KDMesh> Mesh;

		RenderComponent() = default;
		RenderComponent(const RenderComponent&) = delete;
		RenderComponent& operator = (const RenderComponent&) = delete;

		void Bind(KDGraphics& gfx)
		{
			if (!ObjectCBuf)
			{
				ObjectCBuffer.MaterialColor = { 0.2f, 0.8f, 0.3f };
				ObjectCBuf = std::make_unique<PixelConstantBuffer<ObjectCBufferInfo>>(gfx, ObjectCBuffer, 1);
			}

			ObjectCBuf->Update(gfx, ObjectCBuffer);
			ObjectCBuf->Bind(gfx);

			Mesh->Bind(gfx);
		}
	};

	struct PointLightComponent
	{
	private:
		struct LightCBufferInfo
		{
			alignas(16) DirectX::XMFLOAT3 LightPos;
			alignas(16) DirectX::XMFLOAT3 Ambient{ 0.15f, 0.15f, 0.15f };
			alignas(16) DirectX::XMFLOAT3 DiffuseColor{ 1.0f, 1.0f, 1.0f, };
			float DiffuseIntensity = 1.0f;
			float AttenuateConst = 1.0f;
			float AttenuateLatency = 0.045f;
			float AttenuateQuad = 0.0075f;
		};
		std::unique_ptr<PixelConstantBuffer<LightCBufferInfo>> LightCBuf;
	public:
		LightCBufferInfo LightCBuffer{};
	public:
		PointLightComponent() = default;
		PointLightComponent(const PointLightComponent&) = delete;
		PointLightComponent& operator = (const PointLightComponent&) = delete;

		void Bind(KDGraphics& gfx, const DirectX::XMFLOAT3& data)
		{
			if (!LightCBuf)
			{
				LightCBuf = std::make_unique<PixelConstantBuffer<LightCBufferInfo>>(gfx, LightCBuffer);
			}

			LightCBuffer.LightPos = { data.x, data.y, data.z };

			LightCBuf->Update(gfx, LightCBuffer);
			LightCBuf->Bind(gfx);
		}
	};
}