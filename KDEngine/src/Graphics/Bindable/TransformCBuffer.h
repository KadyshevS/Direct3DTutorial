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
		VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
		const Drawable& parent;
	};
}
