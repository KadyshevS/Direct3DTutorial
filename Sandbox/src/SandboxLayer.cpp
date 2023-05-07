#include "SandboxLayer.h"

void SandboxLayer::OnAttach()
{
	Cube = std::make_unique<TestCube>(Window->Graphics());

	Window->Graphics().SetProjection(
		DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
	);
}
void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnUpdate(float ts)
{
	auto dt = Timer.Mark();
	Window->Graphics().ClearBuffer(0.07f, 0.0f, 0.12f);

	Cube->Update(dt);
	Cube->Draw(Window->Graphics());

	Window->Graphics().EndFrame();
}
