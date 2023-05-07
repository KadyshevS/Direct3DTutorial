#include "VertexBuffer.h"

namespace KDE
{
	void VertexBuffer::Bind(KDGraphics& gfx) noexcept
	{
		const UINT offset = 0u;
		Context(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}
}