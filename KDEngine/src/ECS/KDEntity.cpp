#include "KDEntity.h"

#include "Graphics/Bindable/BindableBase.h"
#include "Components.h"

namespace KDE
{
	KDEntity::KDEntity(entt::entity ent, KDScene* scene)
		: m_Scene(scene), m_EntityHandle(ent)
	{
		
	}

	void KDEntity::Bind(KDGraphics& gfx)
	{
		if (HasComponent<CS::TagComponent>())
		{

		}
		if (HasComponent<CS::RenderComponent>())
		{
			auto& rc = GetComponent<CS::RenderComponent>();
			rc.Bind(gfx);
		}
	}
	void KDEntity::Update()
	{
		
	}
}