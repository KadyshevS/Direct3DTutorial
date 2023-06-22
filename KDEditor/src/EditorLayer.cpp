#include "EditorLayer.h"

#include "Graphics/GeoPrimitives.h"
#include "WinBase/GDIPlusManager.h"
#include "Graphics/KDSurface.h"
#include "ECS/Components.h"
#include "imgui/imgui.h"

#include <string>

namespace KDE
{
	KDE::GDIPlusManager gdipm;

	void EditorLayer::OnAttach()
	{
		Scene = std::make_unique<KDScene>(Window->Graphics());
		SceneHierarchy = std::make_shared<SceneHierarchyPanel>(*Scene.get(), Window->Keyboard);
	}
	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(float ts)
	{
		Scene->Draw();
	}
	void EditorLayer::OnImGuiUpdate()
	{
		static bool show_imgui = true;

		//	Input
		if (Window->Keyboard.IsKeyPressed(Key::Control))
		{
			static bool isFirstPress = false;
			if (Window->Keyboard.IsKeyPressed('H'))
			{
				if (!isFirstPress)
				{
					show_imgui = !show_imgui;
					isFirstPress = true;
				}
			}
			else
				isFirstPress = false;
		}
		
		if (show_imgui)
		{
			//	Dockspace
			{
				static bool p_open = true;
				static bool opt_fullscreen = true;
				static bool opt_padding = false;
				static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

				ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
				if (opt_fullscreen)
				{
					const ImGuiViewport* viewport = ImGui::GetMainViewport();
					ImGui::SetNextWindowPos(viewport->WorkPos);
					ImGui::SetNextWindowSize(viewport->WorkSize);
					ImGui::SetNextWindowViewport(viewport->ID);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
					window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
					window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
				}
				else
				{
					dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
				}

				if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
					window_flags |= ImGuiWindowFlags_NoBackground;

				if (!opt_padding)
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImGui::Begin("DockSpace Demo", &p_open, window_flags);
				if (!opt_padding)
					ImGui::PopStyleVar();

				if (opt_fullscreen)
					ImGui::PopStyleVar(2);

				ImGuiIO& io = ImGui::GetIO();
				ImGuiStyle& style = ImGui::GetStyle();
				float minWinSizeX = style.WindowMinSize.x;
				style.WindowMinSize.x = 370.0f;
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}
				style.WindowMinSize.x = minWinSizeX;

				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("File"))
					{
					//	if (ImGui::MenuItem("New"))
					//		NewScene();
					//	if (ImGui::MenuItem("Open...", "Ctrl+O"))
					//		LoadScene();
					//	if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
					//		SaveSceneAs();
					//
					//	ImGui::Separator();

						if (ImGui::MenuItem("Exit", "Alt+F4")) PostQuitMessage(0);

						ImGui::EndMenu();
					}

					ImGui::EndMenuBar();
				}

				ImGui::End();
			}
			{
				static bool p_open = true;
				ImGui::ShowDemoWindow(&p_open);
			}
			{
				SceneHierarchy->OnImGuiRender();
			}
		}
	}
}
