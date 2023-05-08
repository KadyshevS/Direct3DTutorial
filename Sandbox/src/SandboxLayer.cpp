#include "SandboxLayer.h"

#include "Graphics/ECS/Components.h"
#include "Graphics/GeoPrimitives.h"

void SandboxLayer::OnAttach()
{
	Window->Graphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));

	Scene = std::make_unique<KDE::KDScene>(Window->Graphics());
	
	auto entity = Scene->CreateEntity("Test Cube");
	entity.AddComponent<KDE::MeshComponent>(KDE::GP::Cube::Make(Window->Graphics()));
	entity.GetComponent<KDE::TransformComponent>().PositionZ = 2.0f;

	RotX = &entity.GetComponent<KDE::TransformComponent>().RotationX;
	RotZ = &entity.GetComponent<KDE::TransformComponent>().RotationZ;
}
void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnUpdate(float ts)
{
	auto dt = Timer.Mark();
	Window->Graphics().ClearBuffer(0.07f, 0.0f, 0.12f);

	Scene->OnUpdate(Window->Graphics(), dt);

	*RotX += dt;
	*RotZ += dt;

	Window->Graphics().EndFrame();
}
