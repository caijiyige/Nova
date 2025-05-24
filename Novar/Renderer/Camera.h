#pragma once 

#include <glm/glm.hpp>

namespace NV
{
    class Camera
    {
        public:
            Camera() = default;
            Camera(const glm::mat4 projectionMatrix)
                : m_ProjectionMatrix(projectionMatrix)
            {
            }
            Camera(const float degFov, const float width, const float height, const float nearP, const float farP);
            ~Camera() = default;

            void SetProjection(glm::mat4 projectionMatrix){m_ProjectionMatrix = projectionMatrix;}
            const glm::mat4& GetProjectionMatrix() const{ return m_ProjectionMatrix; }

           
        public:
            
            glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

    };
}