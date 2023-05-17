#include "ImGuiManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

namespace KDE
{
	ImGuiManager::ImGuiManager()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
	}
	ImGuiManager::~ImGuiManager()
	{
		ImGui::DestroyContext();
	}
}
