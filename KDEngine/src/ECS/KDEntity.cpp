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
		if (HasComponent<CS::RenderComponent>())
		{
			auto& rc = GetComponent<CS::RenderComponent>();
			rc.Bind(gfx);
		}
		if (HasComponent<CS::PointLightComponent>())
		{
			auto& pos = GetComponent<CS::RenderComponent>().Mesh->Transform.Position;
			auto& plc = GetComponent<CS::PointLightComponent>();

			plc.Bind(gfx, {pos.X, pos.Y, pos.Z});
		}
	}
	void KDEntity::Update()
	{
		
	}
}