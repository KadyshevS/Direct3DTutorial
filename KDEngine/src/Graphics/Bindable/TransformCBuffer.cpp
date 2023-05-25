#include "TransformCBuffer.h"

namespace KDE
{
	std::unique_ptr<KDE::VertexConstantBuffer<TransformCBuffer::Transforms>> TransformCBuffer::pVcbuf;

	TransformCBuffer::TransformCBuffer(KDGraphics& gfx, const Drawable& parent)
		:  parent(parent)
	{
		if (!pVcbuf)
		{
			pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
		}
	}

	void TransformCBuffer::Bind(KDGraphics& gfx)
	{
		using namespace DirectX;

		const auto model = parent.Transform();
		const Transforms tf =
		{
			XMMatrixTranspose(model),
			XMMatrixTranspose(
				model *
				gfx.Projection()
			)
		};

		pVcbuf->Update(gfx, tf);
		pVcbuf->Bind(gfx);
	}
}