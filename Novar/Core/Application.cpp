#include "Application.h"
#include "Log.h"

#include "glfw3.h"
#include "Input.h"

#include "Renderer.h"
#include "Renderer2D.h"

#include "Timestep.h"

namespace NV
{
    Application *Application::s_Instance = nullptr;

  

    Application::Application(const std::string& name)
    
    {
        // Initialization code here
        NV_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create(name));
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();
        Renderer2D::Init();

        m_ImGuiLayer = new ImguiLayer();
        PushLayer(m_ImGuiLayer);   
    }

    Application::~Application()
    {
        // Cleanup code here
    }
    void Application::Run()
    {
        while (m_Running)
        {
            // Update

            float time = glfwGetTime();// PlatForm: GetTime
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;
            if (!m_IsMinisize)
            {
                for (Layer *layer : m_LayerStack)
                {
                    layer->OnUpdate(timestep);
                }
            }
            
            // Render

            m_ImGuiLayer->Begin();
            for (Layer *layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();
            
            m_Window->OnUpdate();

        }
    }
    void Application::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        NV_CORE_TRACE("{0}", e.ToString());

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.m_Handled)
                break;
        }
    }
    void Application::PushLayer(Layer *layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    void Application::PushOverlay(Layer *overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }
    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        m_Running = false;
        return false;
    }
    bool Application::OnWindowResize(WindowResizeEvent &e)
    {
        if (e.GetWidth() ==0 || e.GetHeight() == 0)
        {
            m_IsMinisize = true;
            return false;
        }
        Renderer::OnWindowResize(e.GetWidth() , e.GetHeight());
        m_IsMinisize = false;
        return false;
    }
}
