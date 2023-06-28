#include "Graphics/Bindable/TransformCBuffer.h"

namespace KDE
{
	std::unique_ptr<KDE::VertexConstantBuffer<TransformCBuffer::Transforms>> TransformCBuffer::pVcbuf;

	TransformCBuffer::TransformCBuffer(KDGraphics& gfx, const KDMesh& parent)
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

		const auto modelView = parent.TransformMat() * gfx.Camera.ViewMat();
		const Transforms tf =
		{
			XMMatrixTranspose(modelView),
			XMMatrixTranspose(
				modelView *
				gfx.Camera.ProjectionMat())
		};

		pVcbuf->Update(gfx, tf);
		pVcbuf->Bind(gfx);
	}
}