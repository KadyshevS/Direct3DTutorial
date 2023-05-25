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

	auto ent = Scene->CreateEntity("Test entity");
	
	auto& mesh = ent.GetComponent<CS::RenderComponent>().Mesh;
	auto& pos = mesh.Transform.Position;

	pos.Z = 4.0f;
	mesh = GP::Sphere::Make();

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
}