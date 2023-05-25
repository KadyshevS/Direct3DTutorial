#pragma once

#include "Graphics/Bindable/Bindable.h"
#include "ECS/entt.hpp"

namespace KDE
{
	class KDEntity;

	class KDScene
	{
		friend class KDEntity;
	public:
		KDScene(KDGraphics& gfx);
		KDScene(const KDScene&) = delete;
		KDScene& operator = (const KDScene&) = delete;
		~KDScene() = default;

		KDEntity CreateEntity(const std::string& name = "");
		void DestroyEntity(KDEntity* entity);

		void Bind();
		void Draw();

	protected:
		KDGraphics* m_Graphics = nullptr;
		std::vector<std::unique_ptr<Bindable>> m_Binds;
		entt::registry m_Registry;
	};
}