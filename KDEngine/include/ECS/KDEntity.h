#pragma once

#include "Graphics/KDGraphics.h"
#include "Graphics/Bindable/Bindable.h"
#include "KDScene.h"
#include "ECS/entt.hpp"

#include <cassert>

namespace KDE
{
	class KDEntity
	{
	public:
		KDEntity(entt::entity ent, KDScene* scene);
		KDEntity(const KDEntity&) = default;
		KDEntity& operator = (const KDEntity&) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			assert(!HasComponent<T>() && "This component is already exist.");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			assert(HasComponent<T>() && "This component does not exist.");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			assert(HasComponent<T>() && "Attempt to remove non-existent component");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		entt::entity& EntityHandle() { return m_EntityHandle; }

		void Bind(KDGraphics& gfx);
		void Update();

	private:
		entt::entity m_EntityHandle = entt::null;
		KDScene* m_Scene = nullptr;
	};
}