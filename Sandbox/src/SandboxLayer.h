#pragma once

#include "Core/Layer.h"
#include "Core/KDWindow.h"

class SandboxLayer : public KDE::Layer
{
public:
	SandboxLayer(KDE::KDWindow* window)
		: Layer(*window, "Sandbox Layer") {}
	virtual ~SandboxLayer() {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float ts) override;
private:
	
};