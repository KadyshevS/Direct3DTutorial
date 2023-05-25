#pragma once

#include "Graphics/KDGraphics.h"
#include "Graphics/Bindable/Bindable.h"
#include "KDEntity.h"
#include "ECS/entt.hpp"

namespace KDE
{
	class KDScene
	{
	public:
		KDScene(KDGraphics& gfx);
		KDScene(const KDScene&) = delete;
		KDScene& operator = (const KDScene&) = delete;
		~KDScene() = default;

		KDEntity CreateEntity();

		void Bind();
		void Update();

	private:
		std::vector<Bindable> m_Binds;
		KDGraphics* m_Graphics = nullptr;
		entt::registry m_Registry;
	};
}