#pragma once

#include "Novar/Core/Timestep.h"
#include "Novar/Scene/Entity.h"

#include "Novar/Renderer/EditorCamera.h"


namespace NV
{
    
    class UUID;
    class Physics2D;
    class Scene:public std::enable_shared_from_this<Scene>
    {

        public:
            Scene();
            ~Scene();
            Scene(const std::shared_ptr<Scene>& scene);

            static std::shared_ptr<Scene> CopyScene(const std::shared_ptr<Scene>& scene);

            std::shared_ptr<Entity> CreateEntity(const std::shared_ptr<Entity>& srcEntity);
            std::shared_ptr<Entity> CreateEntity(std::string name);
            std::shared_ptr<Entity> CreateEntityWithUUID(UUID uuid, std::string name);
            void RemoveEntity(std::shared_ptr<Entity> entity);

            void OnRuntimeStart();//Initialize the scene
            void OnRuntimeStop();//Destroy the scene

            void OnSimulationStart();//Initialize the scene
            void OnSimulationStop();//Destroy the scene

	        void OnShowPhysicsCollider(const std::shared_ptr<Camera>& spCamera);

            void OnUpdateRuntime(Timestep ts);//Render the mesh
            void OnUpdateEditor(Timestep ts,const std::shared_ptr<EditorCamera>& camera);//Update the scene in editor mode
            void OnUpdateSimulation(Timestep ts, std::shared_ptr<EditorCamera>& camera);//Update the scene in simulation mode
            void OnViewportResize(uint32_t width, uint32_t height);
            const std::shared_ptr<entt::registry>& GetRegistry() const { return m_spRegistry; }
            std::shared_ptr<entt::registry>& GetRegistry() { return m_spRegistry; }

            std::shared_ptr<Entity> DuplicateEntity(std::shared_ptr<Entity>& entity);

            std::shared_ptr<Entity> GetPrimaryCameraEntity();
            std::shared_ptr<Entity> GetEntityByUUID(UUID uuid);
            std::shared_ptr<Entity> GetEntityByName(const std::string_view& name);
           

            bool IsRunning() const { return m_IsRunning; }

            bool IsPaused() const { return m_IsPaused; }
            void SetPaused(bool paused) { m_IsPaused = paused; }

            void Step(int frames = 1);

            template<typename... Components>
            auto GetAllEntitiesWith()
            {
                return m_spRegistry->view<Components...>();
            }

        private:

            void OnPhysics2DStart();
            void OnPhysics2DStop();
            void OnUpdatePhysics2D(Timestep ts);

            void RenderScene(const std::shared_ptr<Camera>& camera);


        private:
        
            uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

            std::shared_ptr<entt::registry> m_spRegistry;
            std::shared_ptr<Physics2D> m_spPhysics2D;

            std::unordered_map<UUID, std::shared_ptr<Entity>> m_EntityMap;

            bool m_IsRunning = false;
            bool m_IsPaused = false;
            int m_StepFrames = 0;
    };
}