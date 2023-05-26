#pragma once

#include "WinBase/Layer.h"
#include "WinBase/KDWindow.h"
#include "ECS/KDScene.h"
#include "ECS/KDEntity.h"

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
	std::unique_ptr<KDE::KDScene> Scene;
	std::vector<std::unique_ptr<KDE::KDEntity>> Entities;
};