#include "Novar/Renderer/OrthographicCameraControl.h"

#include "Novar/Core/Input.h"
#include "Novar/Core/KeyCodes.h"

namespace NV
{

    OrthographicCameraControl::OrthographicCameraControl(float aspectRatio, bool rotation)
    :m_aspectRatio(aspectRatio),m_Rotation (rotation),m_Camera(-m_aspectRatio * m_ZoomLevel,m_aspectRatio * m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel)
    {
    }
    void OrthographicCameraControl::OnUpdate(Timestep ts)
    {

        
        if(Input::IsKeyPressed(Key::A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts.GetSeconds();
        if(Input::IsKeyPressed(Key::D))
            m_CameraPosition.x += m_CameraTranslationSpeed * ts.GetSeconds();
        if(Input::IsKeyPressed(Key::S))
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts.GetSeconds();
        if(Input::IsKeyPressed(Key::W))
            m_CameraPosition.y += m_CameraTranslationSpeed * ts.GetSeconds();

        

        if (m_Rotation)
        {
            if(Input::IsKeyPressed(Key::Q))
            m_CameraRotation -= m_CameraRotationSpeed * ts.GetSeconds();
            if(Input::IsKeyPressed(Key::E))
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
    void OrthographicCameraControl::OnResize(float width, float height)
    {
        m_aspectRatio = (float)width/(float)height;
        m_Camera.SetProjection(-m_aspectRatio * m_ZoomLevel,m_aspectRatio * m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel);

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
        OnResize(e.GetWidth(), e.GetHeight());

        return false;
    }
}