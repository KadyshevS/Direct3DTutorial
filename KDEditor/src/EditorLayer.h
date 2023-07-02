#pragma once

#include "WinBase/Layer.h"
#include "WinBase/KDWindow.h"
#include "ECS/KDScene.h"
#include "ECS/KDEntity.h"
#include "Graphics/KDMath.h"
#include "Panels/SceneHierarchyPanel.h"
#include "ECS/SceneSerializer.h"

namespace KDE
{
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
		void NewScene();
		void LoadScene();
		void SaveSceneAs();
	private:
		std::unique_ptr<KDScene> Scene;
		std::shared_ptr<SceneHierarchyPanel> SceneHierarchy;
	};
}