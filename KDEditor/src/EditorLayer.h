#pragma once

#include "WinBase/Layer.h"
#include "WinBase/KDWindow.h"
#include "ECS/KDScene.h"
#include "ECS/KDEntity.h"
#include "Graphics/KDMath.h"

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
		std::unique_ptr<KDE::KDScene> Scene;
		std::vector<std::unique_ptr<KDE::KDEntity>> Entities;
		std::unique_ptr<KDE::KDEntity> PointLight;

		DirectX::XMFLOAT2 m_ViewportBounds[2] = { {0.0f, 0.0f}, {0.0f, 0.0f} };
		DirectX::XMFLOAT2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		float rotation = 0.0f;
	};
}