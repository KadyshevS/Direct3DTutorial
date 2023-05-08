#include "KDScene.h"

namespace KDE
{
	KDScene::KDScene()
	{
	}
	KDScene::~KDScene()
	{
	}

	KDEntity KDScene::CreateEntity()
	{
		return KDEntity(entt::entity{}, this);
	}
}