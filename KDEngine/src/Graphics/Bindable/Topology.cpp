#include "Graphics/Bindable/Topology.h"

namespace KDE
{
	Topology::Topology(KDGraphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
		: type(type)
	{}

	void Topology::Bind(KDGraphics& gfx)
	{
		Context(gfx)->IASetPrimitiveTopology(type);
	}
}