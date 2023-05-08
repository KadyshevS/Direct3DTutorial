#include "KDEntity.h"

#include "Components.h"

namespace KDE
{
    KDEntity::KDEntity(entt::entity handle, KDScene* scene)
        : m_Handle(handle), m_Scene(scene)
    {}

    const std::string& KDEntity::DebugName() const
    {
        return m_DebugName;
    }
}