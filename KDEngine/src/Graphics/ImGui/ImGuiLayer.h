#pragma once

#include "WinBase/Layer.h"

namespace KDE
{
	class ImGuiLayer : public KDE::Layer
	{
	public:
		ImGuiLayer(KDE::KDWindow* window);
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}