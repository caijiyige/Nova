#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Event.h"
#include "ApplicationEvent.h"
#include "ImguiLayer.h"

#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"

#include "OrthographicCamera.h"
namespace NV
{
    class NOVA_API Application
    {
    public:
        Application(const std::string& name = "Nova Engine");
        virtual ~Application();
        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline static Application& Get() { return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }

        void Close() { m_Running = false; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
       
        bool m_Running = true;
        bool m_IsMinisize = false;
        float m_LastFrameTime = 0.0f;
        std::unique_ptr<Window> m_Window;
        ImguiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;

        float LastFrameTime = 0.0f;



        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}