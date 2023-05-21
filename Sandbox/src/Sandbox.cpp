#include <WinBase/EntryPoint.h>
#include "SandboxLayer.h"
#include "Graphics/ImGui/ImGuiLayer.h"

class Sandbox : public KDE::KDApplication
{
public:
	Sandbox()
	{
		PushLayer(new SandboxLayer(m_Window.get()));
	}
	~Sandbox() {}
};

KDE::KDApplication* KDE::CreateApplication()
{
	return new Sandbox();
}