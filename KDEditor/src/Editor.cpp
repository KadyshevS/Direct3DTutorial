#include <WinBase/EntryPoint.h>
#include "EditorLayer.h"
#include "Graphics/ImGui/ImGuiLayer.h"

class Editor : public KDE::KDApplication
{
public:
	Editor()
	{
		PushLayer(new EditorLayer(m_Window.get()));
	}
	~Editor() {}
};

KDE::KDApplication* KDE::CreateApplication()
{
	return new Editor();
}