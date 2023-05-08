#pragma once

#include "Graphics/KDMesh.h"
#include "KDScene.h"

#include <memory>

namespace KDE
{
	class KDEntity
	{
	public:
		KDEntity(entt::entity handle, KDScene* scene);
		KDEntity(const KDEntity&) = delete;
		~KDEntity() = default;

		KDEntity& operator = (const KDEntity&) = delete;

		template<typename T>
		void AddComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_Handle);
		}
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_Handle);
		}
	private:
		entt::entity m_Handle = entt::null;
		KDScene* m_Scene;
	};
}