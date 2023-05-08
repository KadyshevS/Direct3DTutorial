#pragma once

#include "EnTT/entt.hpp"
#include "KDEntity.h"

namespace KDE
{
	class KDScene
	{
		friend class KDEntity;
	public:
		KDScene();
		~KDScene();

		KDEntity CreateEntity();
	private:
		entt::registry m_Registry;
	};
}