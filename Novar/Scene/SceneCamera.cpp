#include "Novar/Scene/SceneCamera.h"
#include "glm/ext.hpp"
namespace NV
{
    SceneCamera::SceneCamera()
        : m_position(0.0f), m_rotation(0.0f), m_projectionType(ProjectionType::Perspective)
    {
    }

    SceneCamera::~SceneCamera() = default;

    void SceneCamera::SetViewportSize(float width, float height)
    {
        m_aspectRatio = width / height;
       RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection()
    {
        if (m_projectionType == ProjectionType::Perspective)
        {
            m_ProjectionMatrix = glm::perspective(glm::radians(m_fieldOfView), m_aspectRatio, m_nearClip, m_farClip);
        }
        else
        {
            float left = -m_orthographicSize * m_aspectRatio / 2.0f;
            float right = m_orthographicSize * m_aspectRatio / 2.0f;
            float bottom = -m_orthographicSize / 2.0f;
            float top = m_orthographicSize / 2.0f;
            m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_nearClip, m_farClip);
        }
}

} // namespace NV