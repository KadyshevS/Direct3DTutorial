#pragma once

#include "KDWindow.h"
#include "LayerStack.h"
#include "KDTimer.h"

#include <memory>

namespace KDE
{
	class KDApplication
	{
	public:
		KDApplication(const std::string& name = "KDEngine App");
		virtual ~KDApplication();

		int Run();
		void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	protected:
		std::unique_ptr<KDWindow> m_Window;
		LayerStack m_LayerStack;
		KDTimer m_Timer;

		float m_LastFrameTime = 0.0f;
		bool m_Running = true;
	};

	KDApplication* CreateApplication();
}