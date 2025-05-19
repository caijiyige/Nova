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
        static void BeginScene(const std::shared_ptr<Camera>& camera, const glm::mat4& transform = glm::mat4(1.0f));
        static void BeginScene(OrthographicCamera& camera);
        static void BeginScene(const std::shared_ptr<EditorCamera>& camera);

        static void EndScene();

        static void Flush();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<SubTexture2D>& subtexture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<SubTexture2D>& subtexture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});

        //static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
        //static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});

        static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f,const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f,const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});

        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
        static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

        static void DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID);
        static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID);
        static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID);
    public:
        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;
            uint32_t GetQuadCount() const { return QuadCount; }
            uint32_t GetDrawCalls() const { return DrawCalls; }
            uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
            void Reset() { DrawCalls = 0; QuadCount = 0; }
        } Stats;
        static Statistics GetStats();
        static void ResetStats();
        
    private:
		static void StartBatch();
		static void NextBatch();
        static void FlushAndReset();
    };


}