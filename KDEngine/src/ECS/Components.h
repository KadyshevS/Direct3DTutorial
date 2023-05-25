#pragma once

#include "Graphics/KDGraphics.h"
#include "Graphics/KDMesh.h"

#include <string>

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
		KDMesh Mesh;

		RenderComponent() = default;
		RenderComponent(const RenderComponent&) = default;
		RenderComponent& operator = (const RenderComponent&) = default;

		RenderComponent(KDMesh mesh)
			: Mesh(mesh) {}

		void Bind(KDGraphics& gfx)
		{
			Mesh.Bind(gfx);
		}
	};
}