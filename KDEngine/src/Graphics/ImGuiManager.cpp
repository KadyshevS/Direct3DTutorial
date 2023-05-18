#include "ImGuiManager.h"

#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"

namespace KDE
{
	ImGuiManager::ImGuiManager()
	{
		IMGUI_CHECKVERSION();
		ImGui::StyleColorsDark();
	}
	ImGuiManager::~ImGuiManager()
	{
		
	}

	void ImGuiManager::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiManager::End()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
