#pragma once

#include "Bindable.h"

namespace KDE
{
	class Topology : public Bindable
	{
	public:
		Topology(KDGraphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(KDGraphics& gfx) override;

	protected:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
}
