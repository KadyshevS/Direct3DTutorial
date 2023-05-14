#pragma once

#include "ConstantBuffers.h"
#include "Graphics/Drawable/Drawable.h"

#include <DirectXMath.h>

namespace KDE
{
	class TransformCBuffer : public Bindable
	{
	public:
		TransformCBuffer(KDGraphics& gfx, const Drawable& parent);
		void Bind(KDGraphics& gfx) override;

	private:
		static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcbuf;
		const Drawable& parent;
	};
}
