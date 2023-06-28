#pragma once

#include "ECS/KDEntity.h"
#include "ECS/KDScene.h"
#include "Input/KDKeyboard.h"

namespace KDE
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(KDScene& context, KDKeyboard& keyboardInput);

		void SetContext(KDScene& context);
		void OnImGuiRender();

		std::shared_ptr<KDEntity> GetSelectedEntity() const { return m_SelectionEntity; }
		void SetSelectedEntity(std::shared_ptr<KDEntity> entity);
		void DeleteSelectedEntity();
	private:
		void DrawEntityNode(std::shared_ptr<KDEntity> entity);
		void DrawComponents(std::shared_ptr<KDEntity> entity);
	private:
		KDScene* m_Context;
		KDKeyboard* m_Keyboard;
		std::shared_ptr<KDEntity> m_SelectionEntity;
	};
}