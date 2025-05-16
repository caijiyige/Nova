#include "Novar/Scene/Scene.h"
#include "Novar/Scene/Components.h"
#include "Novar/Scene/Entity.h"
#include "Novar/Scene/ScriptableEntity.h"
#include "entt.hpp"

#include <glm/glm.hpp>

#include "Novar/Core/UUID.h"
#include "Novar/Renderer/Renderer2D.h"

#include "Novar/Physics/Physics2D.h"
#include "Scene.h"


namespace NV
{
	template<typename... Component>
    static void CopyComponent(std::shared_ptr<entt::registry>& src, std::shared_ptr<entt::registry>& dst, const std::unordered_map<UUID,std::shared_ptr<Entity>>& entityMap)
    {
        ([&]()
		{
			auto view = src->view<Component>();
			for (auto srcEntity : view)
			{
				auto dstEntity = entityMap.at(src->get<IDComponent>(srcEntity).ID);

				auto& srcComponent = src->get<Component>(srcEntity);
				dst->emplace_or_replace<Component>(dstEntity->GetEntityHandle(), srcComponent);
			}
		}(), ...);
        
    }
    
    template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, std::shared_ptr<entt::registry>& dst, std::shared_ptr<entt::registry>& src, const std::unordered_map<UUID, std::shared_ptr<Entity>>& enttMap)
	{
		CopyComponent<Component...>(src, dst, enttMap);
	}
/*
	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
		{
			if (src.HasComponent<Component>())
				dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}
*/
    
    std::shared_ptr<Scene> Scene::CopyScene(const std::shared_ptr<Scene> &scene)
    {
        std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
       
        newScene->m_ViewportWidth = scene->m_ViewportWidth;
        newScene->m_ViewportHeight = scene->m_ViewportHeight;
        

        auto& srcRegistry = scene->GetRegistry();
        auto& dstRegistry = newScene->GetRegistry();
        std::unordered_map<UUID, std::shared_ptr<Entity>> entityMap;


        //Create entitys in new scene
        auto idview = srcRegistry->view<IDComponent>();
        for (auto entity : idview)
        {
            auto& idComponent = srcRegistry->get<IDComponent>(entity);
            auto& tagComponent = srcRegistry->get<TagComponent>(entity);
            auto newEntity = newScene->CreateEntityWithUUID(idComponent.ID, tagComponent.Tag);
            entityMap[idComponent.ID] = newEntity;
            
        }

        // Copy components from the original scene to the new scene

        CopyComponent(AllComponents{}, dstRegistry, srcRegistry, entityMap);

        return newScene;
    }
	
    Scene::Scene()
    {
        m_spRegistry = std::make_shared<entt::registry>();
	}
    Scene::~Scene()
    {
        
    }

    Scene::Scene(const std::shared_ptr<Scene> &scene)
    {
		m_ViewportWidth = scene->m_ViewportWidth;
        m_ViewportHeight = scene->m_ViewportHeight;
        

        auto& srcRegistry = scene->GetRegistry();
        auto& dstRegistry = m_spRegistry;
        std::unordered_map<UUID, std::shared_ptr<Entity>> entityMap;


        //Create entitys in new scene
        auto idview = srcRegistry->view<IDComponent>();
        for (auto entity : idview)
        {
            auto& idComponent = srcRegistry->get<IDComponent>(entity);
            auto& tagComponent = srcRegistry->get<TagComponent>(entity);
            auto newEntity = CreateEntityWithUUID(idComponent.ID, tagComponent.Tag);
            entityMap[idComponent.ID] = newEntity;
            
        }

        // Copy components from the original scene to the new scene

        CopyComponent(AllComponents{}, dstRegistry, srcRegistry, entityMap);

    }
    std::shared_ptr<Entity> Scene::CreateEntity(const std::shared_ptr<Entity> &srcEntity)
    {
        return std::shared_ptr<Entity>();
    }

    std::shared_ptr<Entity> Scene::CreateEntity(std::string name)
    {
        return CreateEntityWithUUID(UUID(), name);
    }
    std::shared_ptr<Entity> Scene::CreateEntityWithUUID(UUID uuid, std::string name)
    {
        auto spEntity = std::make_shared<Entity>(m_spRegistry);
        spEntity->AddComponent<IDComponent>(uuid);
        spEntity->AddComponent<TransformComponent>();
		spEntity->AddComponent<TagComponent>(name.empty() ? "Entity" : name);
		m_EntityMap[uuid] = spEntity;
        return spEntity;
        
    }
    void Scene::RemoveEntity(std::shared_ptr<Entity> spEntity)
    {
		m_EntityMap.erase(spEntity->GetUUID());
		spEntity->Destroy();
    }

    void Scene::OnRuntimeStart()
    {
        m_IsRunning = true;
        OnPhysics2DStart();

    }

    void Scene::OnRuntimeStop()
    {
        m_IsRunning = false;
        OnPhysics2DStop();
    }

    void Scene::OnSimulationStart()
    {
        OnPhysics2DStart();
    }

    void Scene::OnSimulationStop()
    {
        OnPhysics2DStop();
    }

    void Scene::OnShowPhysicsCollider(const std::shared_ptr<Camera> &spCamera)
    {
    }

    // std::shared_ptr<Entity> Scene::DuplicateEntity(Entity entity)
    // {
    //     Entity newEntity = CreateEntity(entity.GetName());
        

    //     CopyComponentIfExists(AllComponents{}, newEntity, entity);
	// 	return newEntity;
    // }

   
    void Scene::OnUpdateEditor(Timestep ts,const std::shared_ptr<EditorCamera>& camera)
	{
        
        RenderScene(camera);
	}
    void Scene::OnUpdateRuntime(Timestep ts)
    {
        if (!m_IsPaused || m_StepFrames-- > 0)
		{
			// Physics
			{
			
			}
		}

		// Render 2D
		
		glm::mat4 cameraTransform;
		{
			auto cameraEntitys = m_spRegistry->view<TransformComponent, CameraComponent>();
			for (auto entity : cameraEntitys)
			{
				auto [transform, camera] = cameraEntitys.get<TransformComponent, CameraComponent>(entity);
				
				if (camera.Primary)
				{
					
					cameraTransform = transform.GetTransform();
					camera.Camera.SetProjection(cameraTransform);
					auto cam = std::make_shared<Camera>(camera.Camera);
					RenderScene(cam);
					break;
				}
			}
		}
    }

    
    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
       
        auto view = m_spRegistry->view<CameraComponent>();
        for (auto entity : view)
        {
            auto&& camera = view.get<CameraComponent>(entity);
            if (!camera.FixedAspectRatio)
			{
				camera.Camera.SetViewportSize(width, height);
			}
        }

    }
    void Scene::Step(int frames)
	{
		m_StepFrames = frames;
	}


    std::shared_ptr<Entity> Scene::GetPrimaryCameraEntity()
    {
        auto view = m_spRegistry->view<CameraComponent>();
        for (auto entity : view)
        {
            auto&& camera = view.get<CameraComponent>(entity);
            if (camera.Primary)
            {
                return std::make_shared<Entity>(m_spRegistry, entity);
            }
        }
        return nullptr;
    }

    void Scene::OnPhysics2DStart()
    {
        m_spPhysics2D = std::make_shared<Physics2D>();
        auto components = m_spRegistry->view<Rigidbody2DComponent>();
        for (auto component : components)
        {
            auto spEntity = std::make_shared<Entity>(m_spRegistry, component);
            auto& transform = spEntity->GetComponent<TransformComponent>();
            auto& rigidBody2D = spEntity->GetComponent<Rigidbody2DComponent>();

            m_spPhysics2D->CreateBody(rigidBody2D, transform);

            if (spEntity->HasComponent<BoxCollider2DComponent>())
            {
                auto& boxCollider2D = spEntity->GetComponent<BoxCollider2DComponent>();
                m_spPhysics2D->CreatePolygonShape(boxCollider2D, transform);
            }
            /*
            if (spEntity->HasComponent<CircleCollider2DComponent>())
            {
                auto& circleCollider2D = spEntity->GetComponent<CircleCollider2DComponent>();

                m_spPhysicsSystem2D->CreateCircleShape(circleCollider2D, transform);
            }*/
        }
    }

    void Scene::OnPhysics2DStop()
    {
        m_spPhysics2D = nullptr;
    }

    void Scene::OnUpdatePhysics2D(Timestep ts)
    {

    }

    void Scene::RenderScene(const std::shared_ptr<Camera> &camera)
    {
		Renderer2D::BeginScene(camera);

		// Draw sprites
		{
			auto group = m_spRegistry->group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}
		}
        
		Renderer2D::EndScene();
	}
}