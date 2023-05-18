#pragma once

#include "imgui/imgui.h"

namespace KDE
{
	class ImGuiManager
	{
	public:
		ImGuiManager();
		~ImGuiManager();

		void Begin();
		void End();
	};
}