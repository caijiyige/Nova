#include "Novar/Scene/Entity.h"
#include "Novar/Core/Base.h"
#include "Entity.h"
namespace NV
{

    Entity::Entity(std::shared_ptr<entt::registry> registry, const entt::entity entityHandle)
    : m_spRegistry(registry),m_EntityHandle(entityHandle)
    {
    }
    Entity::Entity(std::shared_ptr<entt::registry> registry)
    : m_spRegistry(registry) 
    {
        m_EntityHandle = m_spRegistry->create();
    }

    Entity::Entity(std::shared_ptr<entt::registry> registry, int entityHandle)
    : m_spRegistry(registry)     
    {
        m_EntityHandle = static_cast<entt::entity>(entityHandle);
    }

    void Entity::Destroy()
    {
        NV_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
        m_spRegistry->destroy(m_EntityHandle);
        m_EntityHandle = entt::null;
    }
}