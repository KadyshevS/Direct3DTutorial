#include "Layer.h"

namespace KDE
{
	Layer::Layer(KDE::KDWindow& window, const std::string& name)
		: m_DebugName(name), Window(&window)
	{}
	Layer::~Layer() {}
}