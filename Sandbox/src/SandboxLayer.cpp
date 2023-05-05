#include "SandboxLayer.h"

#include <cmath>

void SandboxLayer::OnAttach()
{
	
}
void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnUpdate(float ts)
{
	const float c = sin(Timer.Peek()) / 2.0f + 0.5f;
	Window->Graphics().ClearBuffer(c, c, 1.0f);
	Window->Graphics().DrawTestTriangle();
	Window->Graphics().EndFrame();
}
