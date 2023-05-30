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
		//	Dockspace
		{
			static bool p_open = true;
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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
		
		//	Viewport
		{
			ImGui::Begin("Viewport");
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

			ImVec2 vpPanelSize = ImGui::GetContentRegionAvail();

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
			HRESULT result = Window->Graphics().m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pBackBuffer.GetAddressOf()));
			if (SUCCEEDED(result))
			{
				D3D11_TEXTURE2D_DESC desc;
				pBackBuffer->GetDesc(&desc);
				desc.BindFlags = 0;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
				desc.Usage = D3D11_USAGE_STAGING;
				
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

				Microsoft::WRL::ComPtr<ID3D11Texture2D> pImGuiTexture;
				result = Window->Graphics().m_Device->CreateTexture2D(&desc, nullptr, &pImGuiTexture);
				if (SUCCEEDED(result))
				{
					Window->Graphics().m_Context->CopyResource(pImGuiTexture.Get(), pBackBuffer.Get());
					D3D11_MAPPED_SUBRESOURCE resource;
					unsigned int subresource = D3D11CalcSubresource(0, 0, 0);
					HRESULT hr = Window->Graphics().m_Context->Map(pImGuiTexture.Get(), subresource, D3D11_MAP_READ_WRITE, 0, &resource);

					if (SUCCEEDED(hr))
					{
						ImTextureID textureID = resource.pData;

						ImGui::Image(textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
					}				
				}
			}

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();

			ImGui::End();
		}
	}
}
