#include "SandboxLayer.h"

#include "Graphics/GeoPrimitives.h"
#include "WinBase/GDIPlusManager.h"
#include "Graphics/Drawable/ChiliTest.h"
#include "Graphics/Drawable/ChiliTestTextured.h"
#include "ECS/Components.h"
#include "imgui/imgui.h"

#include <string>

using namespace KDE;

KDE::GDIPlusManager gdipm;

void SandboxLayer::OnAttach()
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
	
	Entities[0]->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Cube::Make());
	Entities[1]->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Prism::Make());
	Entities[2]->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Plane::Make());
	Entities[3]->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Cone::Make());
	Entities[4]->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Sphere::Make());

	Entities[0]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = { -2.0f, -1.0f, 4.0f };
	Entities[1]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = {  0.0f, -1.0f, 4.0f };
	Entities[2]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = {  2.0f, -1.0f, 4.0f };
	Entities[3]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = { -1.0f,  1.0f, 4.0f };
	Entities[4]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = {  1.0f,  1.0f, 4.0f };

	for (auto& e : Entities)
	{
		auto& scale = e->GetComponent<CS::RenderComponent>().Mesh->Transform.Scaling;
		scale = {0.8f, 0.8f, 0.8f};
	}

	Scene->Bind();
}
void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(float ts)
{
	Scene->Draw();
}
void SandboxLayer::OnImGuiUpdate()
{
	for (auto& e : Entities)
	{
		auto& mesh = e->GetComponent<CS::RenderComponent>().Mesh;
		auto& pos = mesh->Transform.Position;
		auto& rot = mesh->Transform.Rotation;
		auto& scale = mesh->Transform.Scaling;

		auto& tag = e->GetComponent<CS::TagComponent>().Tag;

		ImGui::Begin(tag.c_str());
		ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&pos), -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat3("Rotation", reinterpret_cast<float*>(&rot), -180.0f, 180.0f, "%.1f");
		ImGui::SliderFloat3("Scale", reinterpret_cast<float*>(&scale), -10.0f, 10.0f, "%.1f");
		ImGui::End();
	}
	{
		auto& cam = Window->Graphics().Camera;

		auto& pos = cam.Transform.Position;
		auto& rot = cam.Transform.Rotation;

		ImGui::Begin("Camera");
		ImGui::SliderFloat3(" Position", reinterpret_cast<float*>(&pos), -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat3(" Rotation", reinterpret_cast<float*>(&rot), -180.0f, 180.0f, "%.1f");
		ImGui::End();
	}
}