#include "Graphics/Bindable/Bindable.h"

#include <stdexcept>

namespace KDE
{
	ID3D11DeviceContext* Bindable::Context(KDGraphics& gfx)
	{
		return gfx.m_Context.Get();
	}
	ID3D11Device* Bindable::Device(KDGraphics& gfx)
	{
		return gfx.m_Device.Get();
	}
	DXGIInfoManager* Bindable::InfoManager(KDGraphics& gfx)
	{
#ifdef _DEBUG
		return &gfx.m_InfoManager;
#else
		throw std::logic_error("Info Manager is unavailable in non-Debug mode.");
#endif
	}
}