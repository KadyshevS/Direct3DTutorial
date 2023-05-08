#include "TransformCBuffer.h"

namespace KDE
{
	std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuffer::pVcbuf;

	TransformCBuffer::TransformCBuffer(KDGraphics& gfx, const Drawable& parent)
		:  parent(parent)
	{
		if (!pVcbuf)
		{
			pVcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
		}
	}

	void TransformCBuffer::Bind(KDGraphics& gfx)
	{
		pVcbuf->Update(gfx,
			DirectX::XMMatrixTranspose(
				parent.Transform() * gfx.Projection()
			)
		);
		pVcbuf->Bind(gfx);
	}
}