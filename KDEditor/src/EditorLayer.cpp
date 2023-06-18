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

		Entities.emplace_back(
			std::make_unique<KDEntity>(Scene->CreateEntity("Cube"))
		);
		Entities.emplace_back(
			std::make_unique<KDEntity>(Scene->CreateEntity("Prism"))
		);
		Entities.emplace_back(
			std::make_unique<KDEntity>(Scene->CreateEntity("Plane"))
		);
		Entities.emplace_back(
			std::make_unique<KDEntity>(Scene->CreateEntity("Cone"))
		);
		Entities.emplace_back(
			std::make_unique<KDEntity>(Scene->CreateEntity("Sphere"))
		);

		PointLight = std::make_unique<KDEntity>(Scene->CreateEntity("Point Light"));
		PointLight->AddComponent<CS::PointLightComponent>();

		Entities[0]->GetComponent<CS::RenderComponent>().Mesh =
			std::make_unique<KDMesh>(GP::Cube::MakeTextured());
		Entities[1]->GetComponent<CS::RenderComponent>().Mesh =
			std::make_unique<KDMesh>(GP::Prism::MakeIndependent(24));
		Entities[2]->GetComponent<CS::RenderComponent>().Mesh =
			std::make_unique<KDMesh>(GP::Plane::Make());
		Entities[3]->GetComponent<CS::RenderComponent>().Mesh =
			std::make_unique<KDMesh>(GP::Cone::MakeIndependent(24));
		Entities[4]->GetComponent<CS::RenderComponent>().Mesh =
			std::make_unique<KDMesh>(GP::Sphere::Make());

		Entities[0]->GetComponent<CS::RenderComponent>().Texture = std::make_unique<KDTexture>(Window->Graphics(), "assets/textures/kappa50.png");

		PointLight->GetComponent<CS::RenderComponent>().Mesh =
			std::make_unique<KDMesh>(GP::Sphere::Make());
		PointLight->GetComponent<CS::RenderComponent>().Mesh->Transform.Scaling = { 0.15f, 0.15f, 0.15f };
		PointLight->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = { 0.0f, 2.0f, 4.0f };

		Entities[0]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = { -2.0f, 0.0f, 4.0f };
		Entities[1]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = { 0.0f, 0.0f, 4.0f };
		Entities[2]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = { 2.0f, 0.0f, 4.0f };
		Entities[3]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = { -1.0f, 0.0f, 2.0f };
		Entities[4]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = { 1.0f, 0.0f, 2.0f };

		for (auto& e : Entities)
		{
			auto& scale = e->GetComponent<CS::RenderComponent>().Mesh->Transform.Scaling;
			scale = { 0.8f, 0.8f, 0.8f };
		}
	}
	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(float ts)
	{
		for (auto& e : Entities)
		{
			auto& rot = e->GetComponent<CS::RenderComponent>().Mesh->Transform.Rotation;
			rot.X = Math::WrapAngle(rot.X + ts * 20.0f);
			rot.Y = Math::WrapAngle(rot.Y + ts * 20.0f);
		}

		PointLight->Bind(Window->Graphics());
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
