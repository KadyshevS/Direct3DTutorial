#pragma once

#include <Graphics/Bindable/TransformCBuffer.h>
#include "Graphics/KDGraphics.h"
#include "Graphics/KDMesh.h"

#include <string>
#include <memory>

namespace KDE::CS
{
	struct TagComponent
	{
		std::string Tag;

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
}