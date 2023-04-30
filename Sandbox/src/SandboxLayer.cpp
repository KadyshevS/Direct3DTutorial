#include "SandboxLayer.h"

#include <sstream>

void SandboxLayer::OnAttach()
{
	
}
void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnUpdate(float ts)
{
	if (Window->Mouse.IsInWindow())
	{
		std::ostringstream oss;
		oss << "Mouse Position: {" << Window->Mouse.PositionX()
			<< ", " << Window->Mouse.PositionY() << "}"
			<< " Frame time = " << ts;

		if (Window->Keyboard.IsKeyPressed(KDE::Key::Control)) oss << " Control key is pressed!";

		Window->SetTitle(oss.str().c_str());
	}
	else
	{
		std::ostringstream oss;
		oss << "Mouse cursor is out of region."
			<< " Frame time = " << ts;

		if (Window->Keyboard.IsKeyPressed(KDE::Key::Control)) oss << " Control key is pressed!";

		Window->SetTitle(oss.str().c_str());
	}
}
