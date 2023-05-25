#include "KDEntity.h"

#include "Graphics/Bindable/BindableBase.h"

namespace KDE
{
	KDEntity::KDEntity(entt::entity ent, KDScene* scene)
		: m_Scene(scene), m_EntityHandle(ent)
	{
		
	}

	void KDEntity::Bind(KDGraphics& gfx)
	{
		
	}
	void KDEntity::Update()
	{

	}
}