#include "SandboxLayer.h"

#include <random>

void SandboxLayer::OnAttach()
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (size_t i = 0; i < 80; i++)
	{
		boxes.push_back(std::make_unique<ChiliTest>(
			Window->Graphics(), rng, adist,
			ddist, odist, rdist
		));
	}
	Window->Graphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}
void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnUpdate(float ts)
{
	auto dt = Timer.Mark();
	Window->Graphics().ClearBuffer(0.07f, 0.0f, 0.12f);

	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(Window->Graphics());
	}

	Window->Graphics().EndFrame();
}
