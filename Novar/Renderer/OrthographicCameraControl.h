#pragma once
#include "OrthographicCamera.h"
#include "Timestep.h"

#include "ApplicationEvent.h"
#include "MouseEvent.h"
namespace NV
{
       class OrthographicCameraControl
    {
        public:
        OrthographicCameraControl(float aspectRatio , bool rotation = false);


        void OnUpdate(Timestep  ts);
        void OnEvent(Event& e);

        OrthographicCamera& GetCamera(){return m_Camera;}
        const OrthographicCamera& GetCamera() const {return m_Camera;}

        float GetZoomLevel() const {return m_ZoomLevel;}
        void SetZoomLevel(float level) {m_ZoomLevel = level; CalculateView();}

        private:
        void CalculateView();
        bool OnMouseScrolled(MouseScrollEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);


        private:


        float m_aspectRatio;//宽高比
        float m_ZoomLevel = 1.0f;//缩放
        OrthographicCamera m_Camera;

        bool m_Rotation;

        glm::vec3 m_CameraPosition ={0.0f, 0.0f,0.0f};
        float m_CameraRotation = 0.0f;
        float m_CameraTranslationSpeed = 5.0f,m_CameraRotationSpeed = 2.5f;

    }; 
}

