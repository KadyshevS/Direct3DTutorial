#pragma once

#include "WinBase/Layer.h"
#include "WinBase/KDWindow.h"
#include "Graphics/Drawable/Drawable.h"

class SandboxLayer : public KDE::Layer
{
public:
	SandboxLayer(KDE::KDWindow* window)
		: Layer(*window, "Sandbox Layer") {}
	virtual ~SandboxLayer() {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float ts) override;
	virtual void OnImGuiUpdate() override;
private:
	
};