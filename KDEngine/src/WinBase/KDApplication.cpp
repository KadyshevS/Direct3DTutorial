#include "WinBase/KDApplication.h"

#include "WinBase/KDWin.h"

namespace KDE
{
	KDApplication::KDApplication(const std::string& name)
	{
		m_Window = std::make_unique<KDWindow>(name.c_str(), 1280, 720);
		m_ImGuiLayer = std::make_unique<ImGuiLayer>(m_Window.get());

		m_Timer.Mark();
	}
	KDApplication::~KDApplication() {}

	int KDApplication::Run()
	{
		for (auto& layer : m_LayerStack)
		{
			layer->OnAttach();
		}
		m_ImGuiLayer->OnAttach();

		while (m_Running)
		{
			if (const auto ecode = m_Window->ProcessMessages())
			{
				return *ecode;
			}

			m_LastFrameTime = m_Timer.Mark();

			m_Window->Graphics().ClearBuffer(0.1f, 0.1f, 0.2f);

			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate(m_LastFrameTime);
			}
			
			m_ImGuiLayer->Begin();
			for (auto& layer : m_LayerStack)
			{
				layer->OnImGuiUpdate();
			}

			m_ImGuiLayer->End();

			m_Window->Graphics().EndFrame();
		}

		for (auto& layer : m_LayerStack)
		{
			layer->OnDetach();
		}
		m_ImGuiLayer->OnDetach();

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