#include "TransformCBuffer.h"

namespace KDE
{
	TransformCBuffer::TransformCBuffer(KDGraphics& gfx, const Drawable& parent)
		: vcbuf(gfx), parent(parent)
	{}

	void TransformCBuffer::Bind(KDGraphics& gfx)
	{
		vcbuf.Update(gfx,
			DirectX::XMMatrixTranspose(
				parent.Transform() * gfx.Projection()
			)
		);
		vcbuf.Bind(gfx);
	}
}