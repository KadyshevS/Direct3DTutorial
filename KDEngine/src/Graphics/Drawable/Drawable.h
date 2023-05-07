#pragma once

#include "Graphics/KDGraphics.h"
#include "Graphics/Bindable/IndexBuffer.h"

#include <DirectXMath.h>
#include <memory>

namespace KDE
{
	class Drawable
	{
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual ~Drawable() = default;

		virtual DirectX::XMMATRIX Transform() const = 0;
		void Draw(KDGraphics& gfx) const;
		virtual void Update(float dt) = 0;
		void AddBind(std::unique_ptr<Bindable> bind);
		void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf);

	private:
		const IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> binds;
	};
}
