#pragma once

#include "Graphics/Bindable/ConstantBuffers.h"
#include "Graphics/KDGraphics.h"
#include "Graphics/Bindable/VertexBuffer.h"
#include "Graphics/Bindable/IndexBuffer.h"
#include "Graphics/KDMesh.h"
#include "KDScene.h"

#include <DirectXMath.h>
#include <memory>
#include <assert.h>

namespace KDE
{
	class KDEntity
	{
		friend class KDScene;
	public:
		KDEntity() = default;
		KDEntity(entt::entity handle, KDScene* scene);
		KDEntity(const KDEntity& other) = default;
		KDEntity& operator = (const KDEntity& other) = default;
		~KDEntity() = default;

		const std::string& DebugName() const;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			assert("Entity already has this component." && !HasComponent<T>());
			return m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
		}
		template<typename T>
		T& GetComponent()
		{
			assert("Entity does not have this component." && HasComponent<T>());
			return m_Scene->m_Registry.get<T>(m_Handle);
		}
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_Handle);
		}
		template<typename T>
		bool RemoveComponent()
		{
			assert("Entity does not have this component." && HasComponent<T>());
			return m_Scene->m_Registry.remove<T>(m_Handle);
		}
	private:
		entt::entity m_Handle = entt::null;
		std::string m_DebugName = "Unnamed Entity";
		KDScene* m_Scene = nullptr;
	};
}