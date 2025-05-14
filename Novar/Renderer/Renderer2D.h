#pragma once

#include "Novar/Renderer/Camera.h"
#include "Novar/Renderer/OrthographicCamera.h"
#include "Novar/Renderer/EditorCamera.h"
#include "Novar/Renderer/Texture.h"
#include "Novar/Renderer/SubTexture2D.h"
#include "Novar/Scene/Components.h"
namespace NV
{
    class Renderer2D
    {
        public:

        static void Init();
        static void ShutDown();
        static void BeginScene(const Camera& camera, const glm::mat4& transform = glm::mat4(1.0f));
        static void BeginScene(OrthographicCamera& camera);
        static void BeginScene(std::shared_ptr<Camera>& camera);

        static void EndScene();

        static void Flush();

        static void DrawQuard(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuard(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuard(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawQuard(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawQuard(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<SubTexture2D>& subtexture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawQuard(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<SubTexture2D>& subtexture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});

        static void DrawQuard(const glm::mat4& transform, const glm::vec4& color);
        static void DrawQuard(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});

        static void DrawRotationQuard(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void DrawRotationQuard(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void DrawRotationQuard(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f,const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawRotationQuard(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f,const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});

        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
        static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuardCount = 0;
            uint32_t GetQuardCount() const { return QuardCount; }
            uint32_t GetDrawCalls() const { return DrawCalls; }
            uint32_t GetTotalVertexCount() const { return QuardCount * 4; }
            uint32_t GetTotalIndexCount() const { return QuardCount * 6; }
            void Reset() { DrawCalls = 0; QuardCount = 0; }
        } Stats;
        static Statistics GetStats();
        static void ResetStats();
        private:
		static void StartBatch();
		static void NextBatch();
        static void FlushAndReset();
    };


}