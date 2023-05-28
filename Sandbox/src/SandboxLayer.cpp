#include "SandboxLayer.h"

#include "Graphics/GeoPrimitives.h"
#include "WinBase/GDIPlusManager.h"
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

	PointLight = std::make_unique<KDEntity>(Scene->CreateEntity("Point Light"));
	PointLight->AddComponent<CS::PointLightComponent>();
	
	Entities[0]->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Cube::MakeIndependent());
	Entities[1]->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Prism::MakeIndependent(24));
	Entities[2]->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Plane::Make());
	Entities[3]->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Cone::MakeIndependent(24));
	Entities[4]->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Sphere::Make());

	PointLight->GetComponent<CS::RenderComponent>().Mesh =
		std::make_unique<KDMesh>(GP::Sphere::Make());
	PointLight->GetComponent<CS::RenderComponent>().Mesh->Transform.Scaling = {0.15f, 0.15f, 0.15f};
	PointLight->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = {0.0f, 2.0f, 4.0f};

	Entities[0]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = { -2.0f, 0.0f, 4.0f };
	Entities[1]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = {  0.0f, 0.0f, 4.0f };
	Entities[2]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = {  2.0f, 0.0f, 4.0f };
	Entities[3]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = { -1.0f, 0.0f, 2.0f };
	Entities[4]->GetComponent<CS::RenderComponent>().Mesh->Transform.Position = {  1.0f, 0.0f, 2.0f };

	for (auto& e : Entities)
	{
		auto& scale = e->GetComponent<CS::RenderComponent>().Mesh->Transform.Scaling;
		scale = { 0.8f, 0.8f, 0.8f };
	}
}
void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(float ts)
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
void SandboxLayer::OnImGuiUpdate()
{
	{
		auto& mesh = PointLight->GetComponent<CS::RenderComponent>().Mesh;
		auto& lightCBuf = PointLight->GetComponent<CS::PointLightComponent>().LightCBuffer;

		auto& pos = mesh->Transform.Position;
		auto& rot = mesh->Transform.Rotation;

		auto& amb = lightCBuf.Ambient;
		auto& dif = lightCBuf.DiffuseColor;
		auto& difInt = lightCBuf.DiffuseIntensity;
		auto& attCst = lightCBuf.AttenuateConst;
		auto& attLat = lightCBuf.AttenuateLatency;
		auto& attQuad = lightCBuf.AttenuateQuad;

		ImGui::Begin("Point Light");
		ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Transform");
		ImGui::DragFloat3("Position", reinterpret_cast<float*>(&pos), 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&rot), 1.0f, 0.0f, 0.0f, "%.2f");
		ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Light");
		ImGui::ColorPicker3("Ambient", reinterpret_cast<float*>(&amb));
		ImGui::ColorPicker3("Diffuse", reinterpret_cast<float*>(&dif));
		ImGui::DragFloat("Intensity", &difInt);
		ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Attenuate");
		ImGui::DragFloat("Constant", &attCst, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("Linear", &attLat, 0.001f, 0.0f, 0.0f, "%.4f");
		ImGui::DragFloat("Quadratic", &attQuad, 0.001f, 0.0f, 0.0f, "%.4f");
		ImGui::End();
	}
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
		ImGui::DragFloat3(" Position", reinterpret_cast<float*>(&pos), 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3(" Rotation", reinterpret_cast<float*>(&rot), 1.0f, 0.0f, 0.0f, "%.2f");
		ImGui::End();
	}
}