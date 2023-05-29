#pragma once

#include "WinBase/Layer.h"
#include "WinBase/KDWindow.h"
#include "ECS/KDScene.h"
#include "ECS/KDEntity.h"

class EditorLayer : public KDE::Layer
{
public:
	EditorLayer(KDE::KDWindow* window)
		: Layer(*window, "Editor Layer") {}
	virtual ~EditorLayer() {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float ts) override;
	virtual void OnImGuiUpdate() override;
private:
	std::unique_ptr<KDE::KDScene> Scene;
	std::vector<std::unique_ptr<KDE::KDEntity>> Entities;
	std::unique_ptr<KDE::KDEntity> PointLight;

	float rotation = 0.0f;
};