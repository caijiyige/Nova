#pragma once 
#include <glm/glm.hpp>
#include "Novar/Renderer/Camera.h"


namespace NV
{
        class SceneCamera : public Camera
            {
            public:
                enum class ProjectionType
                {
                    Perspective = 0,
                    Orthographic = 1
                };
            public:
                SceneCamera();
                SceneCamera(const glm::mat4& projectionMatrix)
                    : Camera(projectionMatrix)
                {
                }
                ~SceneCamera();

                void SetPosition(const glm::vec3& position) { m_position = position; }
                void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; }
                const glm::vec3& GetPosition() const { return m_position; }
                const glm::vec3& GetRotation() const { return m_rotation; }
                
                void SetAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; RecalculateProjection(); }
                float GetAspectRatio() const { return m_aspectRatio; }
                void SetFieldOfView(float fov) { m_fieldOfView = fov;  RecalculateProjection(); }
                float GetFieldOfView() const { return m_fieldOfView; }
                void SetOrthographicSize(float size) { m_orthographicSize = size;  RecalculateProjection(); }
                float GetOrthographicSize() const { return m_orthographicSize; }
                void SetNearClip(float nearClip) { m_nearClip = nearClip;  RecalculateProjection(); }
                float GetNearClip() const { return m_nearClip; }
                void SetFarClip(float farClip) { m_farClip = farClip;  RecalculateProjection(); }
                float GetFarClip() const { return m_farClip; }
                void SetOrthographic(float size, float nearClip, float farClip)
                {
                    m_orthographicSize = size;
                    m_nearClip = nearClip;
                    m_farClip = farClip;
                     RecalculateProjection(); 
                }
                void SetViewportSize(float width, float height) ;

                void SetProjectionType(ProjectionType type) { m_projectionType = type; }
                ProjectionType GetProjectionType() const { return m_projectionType; }


            private:
                void RecalculateProjection();

            private:

                glm::vec3 m_position;
                glm::vec3 m_rotation;
                ProjectionType m_projectionType = ProjectionType::Orthographic;

                float m_aspectRatio = 1.0f;
                float m_fieldOfView = 45.0f;
                float m_orthographicSize = 10.0f;
                float m_nearClip = -1.0f;
                float m_farClip = 1.0f;
            };
} // namespace NV