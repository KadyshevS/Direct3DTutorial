#pragma once

#include "WinBase/Layer.h"
#include "WinBase/KDWindow.h"
#include "WinBase/KDTimer.h"
#include "Graphics/Drawable/TestCube.h"

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
	KDE::KDTimer Timer;
	std::unique_ptr<TestCube> Cube;
};