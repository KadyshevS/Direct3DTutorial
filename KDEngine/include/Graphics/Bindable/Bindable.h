#pragma once

#include "Graphics/KDGraphics.h"

namespace KDE
{
	class Bindable
	{
	public:
		virtual void Bind(KDGraphics& gfx) = 0;
		virtual ~Bindable() = default;
	protected:
		static ID3D11DeviceContext* Context(KDGraphics& gfx);
		static ID3D11Device* Device(KDGraphics& gfx);
		static DXGIInfoManager* InfoManager(KDGraphics& gfx);
	};
}