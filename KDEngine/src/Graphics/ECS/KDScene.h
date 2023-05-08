#pragma once

#include "EnTT/entt.hpp"
#include "Graphics/Bindable/Bindable.h"
#include "Graphics/KDGraphics.h"

namespace KDE
{
	class KDEntity;

	class KDScene
	{
		friend class KDEntity;
	public:
		KDScene(KDGraphics& gfx);
		~KDScene();

		KDEntity CreateEntity(const std::string& name);

		void OnUpdate(KDGraphics& gfx, float dt);
	private:
		entt::registry m_Registry;
		std::vector<std::unique_ptr<Bindable>> binds;
	};
}