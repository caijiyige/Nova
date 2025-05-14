#pragma once
#include "Novar/Core/Base.h"
#include "Novar/Scene/Components.h"
#include <entt.hpp>

namespace NV
{
    class Entity
    {

    public:
        Entity() = default;
        Entity(std::shared_ptr<entt::registry> registry);
        Entity(std::shared_ptr<entt::registry> registry, const entt::entity entityHandle);
        Entity(std::shared_ptr<entt::registry> registry, int entityHandle);
        
        void Destroy();

    public:
    
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            NV_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            return m_spRegistry->emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T, typename... Args>
        T& AddOrReplaceComponent(Args&&... args)
        {
            NV_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            return m_spRegistry->emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            NV_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_spRegistry->get<T>(m_EntityHandle);
        }
        template<typename T>
        const T& GetComponent() const 
        {
            NV_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_spRegistry->get<T>(m_EntityHandle);
        }
    

        template<typename T>
        bool HasComponent()
        {
            return m_spRegistry->has<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            NV_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            m_spRegistry->remove<T>(m_EntityHandle);
        }

        operator bool () const { return m_EntityHandle != entt::null; }
        
        bool operator==(const Entity& other) const
        {
            return m_EntityHandle == other.m_EntityHandle && m_spRegistry == other.m_spRegistry;
        }

        
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }
        operator entt::entity() const { return m_EntityHandle; }

        UUID GetUUID() const{ return GetComponent<IDComponent>().ID;}
        entt::entity GetEntityHandle() const { return m_EntityHandle; }
        const std::string& GetName() {return GetComponent<TagComponent>().Tag;}

    private:
        entt::entity m_EntityHandle ;
        std::shared_ptr<entt::registry> m_spRegistry;
    };
     
}