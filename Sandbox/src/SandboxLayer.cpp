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
	Window->Graphics().SetProjection(
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f) *
		DirectX::XMMatrixPerspectiveLH(1.0f, (float)Window->Height() / (float)Window->Width(), 0.5f, 40.0f));

	Scene = std::make_unique<KDScene>(Window->Graphics());

	Entity = std::make_unique<KDE::KDEntity>(Scene->CreateEntity("Test entity"));
	
	auto& mesh = Entity->GetComponent<CS::RenderComponent>().Mesh;
	mesh = std::make_unique<KDE::KDMesh>(GP::Sphere::Make());

	auto& pos = mesh->Transform.Position;
	
	pos.Z = -2.0f;

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
	auto& mesh = Entity->GetComponent<CS::RenderComponent>().Mesh;
	auto& pos = mesh->Transform.Position;
	auto& rot = mesh->Transform.Rotation;
	auto& scale = mesh->Transform.Scaling;

	ImGui::Begin("Entity Transform");
	ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&pos), -10.0f, 10.0f, "%.1f");
	ImGui::SliderFloat3("Rotation", reinterpret_cast<float*>(&rot), -180.0f, 180.0f, "%.1f");
	ImGui::SliderFloat3("Scale", reinterpret_cast<float*>(&scale), -10.0f, 10.0f, "%.1f");
	ImGui::End();
}