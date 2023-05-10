#include "SandboxLayer.h"

#include "Graphics/ECS/Components.h"
#include "Graphics/GeoPrimitives.h"

#include <string>

void SandboxLayer::OnAttach()
{
	Window->Graphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));

	Scene = std::make_unique<KDE::KDScene>(Window->Graphics());
	
	Entities[0] = Scene->CreateEntity("Test Sphere");
	Entities[0].AddComponent<KDE::MeshComponent>(KDE::GP::Sphere::MakeTesselated(20, 20));

	Entities[1] = Scene->CreateEntity("Test Cone");
	Entities[1].AddComponent<KDE::MeshComponent>(KDE::GP::Cone::MakeTesselated(20));

	Entities[2] = Scene->CreateEntity("Test Cube");
	Entities[2].AddComponent<KDE::MeshComponent>(KDE::GP::Cube::Make());

	Entities[3] = Scene->CreateEntity("Test Prism");
	Entities[3].AddComponent<KDE::MeshComponent>(KDE::GP::Prism::MakeTesselated(20));

	Entities[4] = Scene->CreateEntity("Test Plane");
	Entities[4].AddComponent<KDE::MeshComponent>(KDE::GP::Plane::Make());

	auto& pos  = Entities[0].GetComponent<KDE::TransformComponent>().Position;
	auto& pos2 = Entities[1].GetComponent<KDE::TransformComponent>().Position;
	auto& pos3 = Entities[2].GetComponent<KDE::TransformComponent>().Position;
	auto& pos4 = Entities[3].GetComponent<KDE::TransformComponent>().Position;
	auto& pos5 = Entities[4].GetComponent<KDE::TransformComponent>().Position;

	pos = { 0.0f, -1.0f, 4.0f };
	pos2 = { 2.0f, -1.0f, 4.0f };
	pos3 = { -2.0f, -1.0f, 4.0f };
	pos4 = { -1.0f, 1.0f, 4.0f };
	pos5 = { 1.0f, 1.0f, 4.0f };
}
void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnUpdate(float ts)
{
	auto dt = Timer.Mark();
	Window->Graphics().ClearBuffer(0.07f, 0.0f, 0.12f);

	Scene->OnUpdate(Window->Graphics(), dt);

	for (size_t i = 0; i < 5; i++)
	{
		auto& rot = Entities[i].GetComponent<KDE::TransformComponent>().Rotation;
		rot.X = KDE::WrapAngle(rot.X + dt);
		rot.Y = KDE::WrapAngle(rot.Y + dt);
	}

	Window->Graphics().EndFrame();
}
