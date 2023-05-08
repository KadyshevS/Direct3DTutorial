#pragma once

#include "Graphics/KDMesh.h"
#include "KDGraphics.h"

#include <vector>

namespace KDE::GP
{
	class Cube
	{
	public:
		static KDMesh Make(KDGraphics& gfx)
		{
			const std::vector<Vertex> vertices = {
				{ -0.5f, -0.5f, -0.5f },
				{  0.5f, -0.5f, -0.5f },
				{  0.5f,  0.5f, -0.5f },
				{ -0.5f,  0.5f, -0.5f },
				{ -0.5f, -0.5f,  0.5f },
				{  0.5f, -0.5f,  0.5f },
				{  0.5f,  0.5f,  0.5f },
				{ -0.5f,  0.5f,  0.5f }
			};
			
			const std::vector<uint32_t> indices = {
				0, 3, 1,	1, 3, 2,
				5, 7, 4,	5, 6, 7,
				4, 7, 3,	4, 3, 0,
				2, 6, 5,	2, 5, 1,
				3, 7, 6,	3, 6, 2,
				0, 5, 4,	0, 1, 5
			};

			return { gfx, vertices, indices };
		}
	};
}