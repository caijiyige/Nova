#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
namespace NV
{
    class Renderer2D
    {
        public:

        static void Init();
        static void ShutDown();
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Flush();

        static void DrawQuard(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuard(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuard(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawQuard(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawQuard(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<SubTexture2D>& subtexture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawQuard(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<SubTexture2D>& subtexture,float Tiling = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});

        static void DrawRotationQuard(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void DrawRotationQuard(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void DrawRotationQuard(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f,const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
        static void DrawRotationQuard(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture,float Tiling = 1.0f,const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});

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

        static void FlushAndReset();
    };


}