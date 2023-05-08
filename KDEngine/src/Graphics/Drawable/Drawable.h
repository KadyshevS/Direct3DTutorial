#pragma once

#include "Graphics/KDGraphics.h"
#include "Graphics/Bindable/IndexBuffer.h"

#include <DirectXMath.h>
#include <memory>

namespace KDE
{
	class Drawable
	{
		template<typename T>
		friend class DrawableBase;
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual ~Drawable() = default;

		virtual DirectX::XMMATRIX Transform() const = 0;
		void Draw(KDGraphics& gfx) const;
		virtual void Update(float dt) = 0;

	protected:
		void AddBind(std::unique_ptr<Bindable> bind);
		void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf);

	private:
		virtual const std::vector<std::unique_ptr<Bindable>>& StaticBinds() const = 0;
	private:
		const IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> binds;
	};
}
