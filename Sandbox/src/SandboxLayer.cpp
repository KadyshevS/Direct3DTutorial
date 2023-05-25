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


}
void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(float ts)
{
}
void SandboxLayer::OnImGuiUpdate()
{
}