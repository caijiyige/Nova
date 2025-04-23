#include "OrthographicCameraControl.h"

#include "Input.h"
#include "KeyCodes.h"

namespace NV
{

    OrthographicCameraControl::OrthographicCameraControl(float aspectRatio, bool rotation)
    :m_aspectRatio(aspectRatio),m_Rotation (rotation),m_Camera(-m_aspectRatio * m_ZoomLevel,m_aspectRatio * m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel)
    {
    }
    void OrthographicCameraControl::OnUpdate(Timestep ts)
    {

        
        if(Input::IsKeyPressed(NOVA_KEY_A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts.GetSeconds();
        if(Input::IsKeyPressed(NOVA_KEY_D))
            m_CameraPosition.x += m_CameraTranslationSpeed * ts.GetSeconds();
        if(Input::IsKeyPressed(NOVA_KEY_S))
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts.GetSeconds();
        if(Input::IsKeyPressed(NOVA_KEY_W))
            m_CameraPosition.y += m_CameraTranslationSpeed * ts.GetSeconds();

        

        if (m_Rotation)
        {
            if(Input::IsKeyPressed(NOVA_KEY_Q))
            m_CameraRotation -= m_CameraRotationSpeed * ts.GetSeconds();
            if(Input::IsKeyPressed(NOVA_KEY_E))
            m_CameraRotation += m_CameraRotationSpeed * ts.GetSeconds();
            m_Camera.SetRotation(m_CameraRotation);
            
        }
        m_Camera.SetPosition(m_CameraPosition);

    }
    void OrthographicCameraControl::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT_FN(OrthographicCameraControl::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraControl::OnWindowResized));

    }
    void OrthographicCameraControl::CalculateView()
    {
        m_Camera.SetProjection(-m_aspectRatio * m_ZoomLevel,m_aspectRatio * m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel);
    }
    bool OrthographicCameraControl::OnMouseScrolled(MouseScrollEvent &e)
    {
        m_ZoomLevel -= e.GetYOffset();
        m_ZoomLevel = std::max(m_ZoomLevel, static_cast<float>(0.25));
        m_Camera.SetProjection(-m_aspectRatio * m_ZoomLevel,m_aspectRatio * m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel);
        return false;
    }
    bool OrthographicCameraControl::OnWindowResized(WindowResizeEvent &e)
    {
        m_aspectRatio = (float)e.GetWidth()/(float)e.GetHeight();
        m_Camera.SetProjection(-m_aspectRatio * m_ZoomLevel,m_aspectRatio * m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel);

        return false;
    }
}