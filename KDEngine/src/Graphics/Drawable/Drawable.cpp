#include "Drawable.h"

#include "Graphics/GraphicsThrowMacros.h"
#include "Graphics/Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

namespace KDE
{
	void Drawable::Draw(KDGraphics& gfx) const
	{
		for (auto& b : binds)
		{
			b->Bind(gfx);
		}
		gfx.DrawIndexed(pIndexBuffer->Count());
	}

	void Drawable::AddBind(std::unique_ptr<Bindable> bind)
	{
		assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		binds.push_back(std::move(bind));
	}
	void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf)
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		binds.push_back(std::move(ibuf));
	}
}