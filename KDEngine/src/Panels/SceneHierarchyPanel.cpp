#include "Panels/SceneHierarchyPanel.h"

#include "ECS/Components.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <DirectXMath.h>

namespace KDE
{
	SceneHierarchyPanel::SceneHierarchyPanel(KDScene& context, KDKeyboard& keyboardInput)
		: m_Keyboard(&keyboardInput)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(KDScene& context)
	{
		m_Context = &context;
		m_SelectionEntity = {};
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_SelectionEntity = std::make_shared<KDEntity>(m_Context->CreateEntity("Empty Entity"));

			ImGui::EndPopup();
		}

		m_Context->m_Registry.each(
			[&](auto entityID)
			{
				KDEntity entity{ entityID, m_Context };
				DrawEntityNode(std::make_shared<KDEntity>(entity));
			}
		);

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionEntity = {};

		if (m_Keyboard->IsKeyPressed(Key::Delete) && m_SelectionEntity)
			DeleteSelectedEntity();

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionEntity)
		{
			DrawComponents(m_SelectionEntity);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(std::shared_ptr<KDEntity> entity)
	{
		m_SelectionEntity = entity;
	}
	void SceneHierarchyPanel::DeleteSelectedEntity()
	{
		if (m_SelectionEntity)
		{
			m_Context->DestroyEntity(*m_SelectionEntity);
			m_SelectionEntity = {};
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(std::shared_ptr<KDEntity> entity)
	{
		ImGuiTreeNodeFlags flags = ((m_SelectionEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		auto& tag = entity->GetComponent<KDE::CS::TagComponent>().Tag;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity->EntityHandle(), flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionEntity = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			bool openedEx = ImGui::TreeNodeEx((void*)((uint64_t)(uint32_t)entity->EntityHandle() + 1000), flags, tag.c_str());
			ImGui::TreePop();
			if (openedEx)
				ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(*entity);
			if (m_SelectionEntity == entity)
				m_SelectionEntity = {};
		}
	}
	void DrawVec3Control(const std::string& label, DirectX::XMFLOAT3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	void DrawFileSelectControl(const std::string& label, const std::string& value)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGui::PushID("LoadedMesh");

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 165.0f, lineHeight };

		if (ImGui::Button(value.c_str(), buttonSize));

		ImVec2 buttonSize2 = { lineHeight + 25.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f });

		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Load", buttonSize2));
		ImGui::PopFont();
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(std::shared_ptr<KDEntity> entity)
	{
		auto flags = ImGuiTreeNodeFlags_DefaultOpen;

		if (entity->HasComponent<KDE::CS::TagComponent>())
		{
			auto& tag = entity->GetComponent<KDE::CS::TagComponent>().Tag;

			char buf[256];
			memset(buf, 0, sizeof(buf));
			strcpy_s(buf, tag.c_str());
			if (ImGui::InputText("##Tag", buf, sizeof(buf)))
			{
				tag = std::string(buf);
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			//	if (ImGui::MenuItem("Camera"))
			//	{
			//		m_SelectionEntity.AddComponent<KDE::CS::CameraComponent>();
			//		ImGui::CloseCurrentPopup();
			//	}

			if (ImGui::MenuItem("Point Light"))
			{
				m_SelectionEntity->AddComponent<KDE::CS::PointLightComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (entity->HasComponent<KDE::CS::RenderComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(KDE::CS::RenderComponent).hash_code(), flags, "Transform");
			if (open)
			{
				auto& tc = entity->GetComponent<KDE::CS::RenderComponent>().Mesh->Transform;
				
				DrawVec3Control("Position", reinterpret_cast<DirectX::XMFLOAT3&>(tc.Position));
				DrawVec3Control("Rotation", reinterpret_cast<DirectX::XMFLOAT3&>(tc.Rotation));
				DrawVec3Control("Scale", reinterpret_cast<DirectX::XMFLOAT3&>(tc.Scaling), 1.0f);

				ImGui::TreePop();
			}

			bool open2 = ImGui::TreeNodeEx((void*)typeid(KDE::CS::TagComponent).hash_code(), flags, "Render");
			if(open2)
			{
				auto& mesh = entity->GetComponent<KDE::CS::RenderComponent>().Mesh;

				DrawFileSelectControl("Mesh", mesh->Name);

				ImGui::TreePop();
			}
		}
	}
}