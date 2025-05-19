#pragma once
#include "Novar/Core/UUID.h"
#include "Novar/Renderer/Texture.h"
#include "Novar/Scene/SceneCamera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>


namespace NV
{
    struct IDComponent
    {
        /* data */
        UUID ID = UUID();
        IDComponent() = default;
        IDComponent(const IDComponent&)= default;
        IDComponent(const UUID& id)
            : ID(id){}

        operator const UUID&() const { return ID; }
        operator UUID() { return ID; }
        
    };

    struct TagComponent
    {
        /* data */
        std::string Tag = "Entity";
        TagComponent() = default;
        

        TagComponent(const TagComponent&)= default;
        
        TagComponent(const std::string& tag)
            : Tag(tag){}

        operator const std::string&() const { return Tag; }
        operator std::string() { return Tag; }
        
    };

    struct TransformComponent
    {
        /* data */
       

        glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f};
        TransformComponent() = default;
        TransformComponent(const TransformComponent&)= default;
        

        glm::mat4 GetTransform() const
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), Translation);
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1.0f, 0.0f, 0.0f});
            rotation = glm::rotate(rotation, Rotation.y, {0.0f, 1.0f, 0.0f});
            rotation = glm::rotate(rotation, Rotation.z, {0.0f, 0.0f, 1.0f});
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);
            return translation * rotation * scale;
        }
        
        
    };

    struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		std::shared_ptr<Texture2D> Texture;
		float Tiling = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};
	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

    

    struct CameraComponent
    {
        /* data */
        SceneCamera  Camera;
        bool Primary = false;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&)= default;

        
    };


    struct ScriptComponent
	{
		std::string ClassName;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

	// Forward declaration
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

    // Physics
	const std::array<std::string, 3> BodyTypeStrings = { "Static","Dynamic","Kinematic" };
	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		// TODO(Yan): move into physics material in the future maybe
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		// TODO(Yan): move into physics material in the future maybe
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

    template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = 
		    ComponentGroup<TransformComponent,
			CameraComponent, ScriptComponent,
			NativeScriptComponent, Rigidbody2DComponent, BoxCollider2DComponent>;


    
}