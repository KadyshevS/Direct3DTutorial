#include "PointLight.h"

#include "imgui/imgui.h"

PointLight::PointLight(KDE::KDGraphics& gfx, float radius)
	:
	mesh(KDE::GP::Sphere::Make()),
	cbuf(gfx)
{}

void PointLight::SpawnControlWindow()
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset()
{
	pos = { 0.0f,0.0f,0.0f };
}

void PointLight::Draw(KDE::KDGraphics& gfx) const
{
	mesh.SetPos(pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(KDE::KDGraphics& gfx) const
{
	cbuf.Update(gfx, PointLightCBuf{ pos });
}