#include "SandboxLayer.h"

#include "Graphics/GeoPrimitives.h"
#include "WinBase/GDIPlusManager.h"
#include "Graphics/Drawable/ChiliTest.h"
#include "Graphics/Drawable/ChiliTestTextured.h"
#include "imgui/imgui.h"

#include <string>

KDE::GDIPlusManager gdipm;

void SandboxLayer::OnAttach()
{
	Window->Graphics().SetProjection(
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f) *
		DirectX::XMMatrixPerspectiveLH(1.0f, (float)Window->Height() / (float)Window->Width(), 0.5f, 40.0f));

	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> adist{ 0.0f,KDE::Math::PI * 2.0f };
	std::uniform_real_distribution<float> ddist{ 0.0f,KDE::Math::PI * 0.5f };
	std::uniform_real_distribution<float> odist{ 0.0f,KDE::Math::PI * 0.08f };
	std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
	std::uniform_real_distribution<float> bdist{ 0.4f,3.0f }; 
	std::uniform_int_distribution<int> edist{ 0,4 }; 

	Entities.resize(180);

	for (int i = 0; i < 180; i++)
	{
		std::unique_ptr<KDE::KDMesh> mesh;

		switch (edist(rng))
		{
			case 0:
			{
				mesh = std::make_unique<KDE::KDMesh>(KDE::GP::Cone::Make());
				Entities[i] = std::make_unique<ChiliTest>(Window->Graphics(), *mesh.get(), rng, adist, ddist, odist, rdist);
				break;
			}
			case 1:
			{
				mesh = std::make_unique<KDE::KDMesh>(KDE::GP::Cube::Make());
				Entities[i] = std::make_unique<ChiliTest>(Window->Graphics(), *mesh.get(), rng, adist, ddist, odist, rdist);
				break;
			}
			case 2:
			{
				mesh = std::make_unique<KDE::KDMesh>(KDE::GP::Plane::MakeTextured());
				Entities[i] = std::make_unique<ChiliTestTextured>(Window->Graphics(), *mesh.get(), "assets\\textures\\kappa50.png", rng, adist, ddist, odist, rdist);
				break;
			}
			case 3:
			{
				mesh = std::make_unique<KDE::KDMesh>(KDE::GP::Prism::Make());
				Entities[i] = std::make_unique<ChiliTest>(Window->Graphics(), *mesh.get(), rng, adist, ddist, odist, rdist);
				break;
			}
			case 4:
			{
				mesh = std::make_unique<KDE::KDMesh>(KDE::GP::Sphere::Make());
				Entities[i] = std::make_unique<ChiliTest>(Window->Graphics(), *mesh.get(), rng, adist, ddist, odist, rdist);
				break;
			}
		}
	}
}
void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnUpdate(float ts)
{
	for (auto& e : Entities)
	{
		e->Update(Window->Keyboard.IsKeyPressed(KDE::Key::Shift) ? 0.0f : ts);
		e->Draw(Window->Graphics());
	}
}
void SandboxLayer::OnImGuiUpdate()
{
	static bool show_demo = true;
	if (show_demo)
		ImGui::ShowDemoWindow(&show_demo);

	ImGui::Begin("Test Window");
	ImGui::TextColored({ 0.2, 0.8, 0.3, 1.0 }, "Hello ImGui!");
	ImGui::End();
}