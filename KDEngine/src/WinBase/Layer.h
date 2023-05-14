#pragma once

#include "KDWindow.h"

#include <string>

namespace KDE
{
	class Layer
	{
	public:
		Layer(KDE::KDWindow& window, const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float dt) {}

		const std::string& Name() const { return m_DebugName; }

		KDWindow* Window;
		
	protected:
		std::string m_DebugName;
	};
}