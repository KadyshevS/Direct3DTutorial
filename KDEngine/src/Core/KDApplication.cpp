#include "KDApplication.h"

#include "KDWin.h"

namespace KDE
{
	KDApplication::KDApplication(const std::string& name)
	{
		m_Window = std::make_unique<KDWindow>(name.c_str(), 800, 600);

		m_Timer.Mark();
	}
	KDApplication::~KDApplication() {}

	int KDApplication::Run()
	{
		for (auto& layer : m_LayerStack)
		{
			layer->OnAttach();
		}

		while (m_Running)
		{
			Sleep(1);	// TODO: This is for fix CPU freezing while updating window. Need to find another solution  (!!!)

			if (const auto ecode = m_Window->ProcessMessages())
			{
				return *ecode;
			}

			m_LastFrameTime = m_Timer.Mark();
			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate(m_LastFrameTime);
			}
		}

		return 0;
	}
	void KDApplication::Close()
	{
		m_Running = false;
	}

	void KDApplication::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	void KDApplication::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}
}