
#include "EditerLayer.h"
#include "Instrumentor.h"
#include "imgui.h"
#include "ext.hpp"

namespace NV
{
    EditerLayer::EditerLayer()
        : Layer("EditerLayer"),m_OrthoCameraControl(1280.0f/960.0f,true)
        {
        }
    void EditerLayer::OnAttach()
    {
       
        m_Texture2D = NV::Texture2D::Create("F:/LearnGameEngine/Nova/SandBox/assert/textures/Checkerboard.png");
        NV::FrameBufferSpecification spec;

        spec.Width = 1280;
        spec.Height = 720;

        m_FrameBuffer = FrameBuffer::Create(spec);
        m_FrameBuffer->Bind();
    }

    void EditerLayer::OnDetach()
    {
    }

    void EditerLayer::OnUpdate(NV::Timestep ts)
    {
            NV_PROFILE_FUNCTION();
            
            {
                NV_PROFILE_SCOPE("m_OrthoCameraControl::OnUpdate");
                m_OrthoCameraControl.OnUpdate(ts);
            }


            NV::Renderer2D::ResetStats();
            
        
            m_FrameBuffer->Bind();
            NV::RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0});
            NV::RendererCommand::Clear();
            {
                NV_PROFILE_SCOPE("Renderer2D::DrawQuard");
                static float rotation = 0.0f;
                rotation += ts.GetMillseconds() * 50.0f;
                
                Renderer2D::BeginScene(m_OrthoCameraControl.GetCamera());
                Renderer2D::DrawQuard({1.0f,1.0f},{1.0f,1.0f},{0.1f, 0.2f, 0.8f, 1.0f});
                
                //NV::Renderer2D::DrawRotationQuard({-0.5f,-0.5f},{0.5f,0.5f},glm::radians(45.0f),{0.8f, 0.2f, 0.1f, 1.0f});
                Renderer2D::DrawQuard({0.0f,0.0f,-0.1f},{10.0f,10.0f},m_Texture2D,10.0f);
                Renderer2D::DrawRotationQuard({0.0f,0.0f,-0.1f},{10.0f,10.0f},rotation,m_Texture2D,10.0f,{0.8f,0.5f,0.3f,0.5f});
                Renderer2D::EndScene();

                Renderer2D::BeginScene(m_OrthoCameraControl.GetCamera());

                for(float y = -5.0f; y<5.0f; y+=0.1f)
                {
                    for(float x = -5.0f; x<5.0f; x+=0.1f)
                    {
                        glm::vec4 color = {x/5.0f,y/5.0f,0.0f,0.5f};
                       Renderer2D::DrawQuard({x,y,-0.2f},{0.45f,0.45f},color);
                    }
                }
                
                Renderer2D::EndScene();

                m_FrameBuffer->UnBind();
            }
            
    }

    void EditerLayer::OnEvent(Event &e)
    {
        m_OrthoCameraControl.OnEvent(e);
    }

    void EditerLayer::OnImGuiRender()
    {
        NV_PROFILE_FUNCTION();

        // Note: Switch this to true to enable dockspace
        static bool dockingEnabled = true;
        if (dockingEnabled)
        {
            static bool dockspaceOpen = true;
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows, 
                    // which we can't undo at the moment without finer window depth/z control.
                    //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                    if (ImGui::MenuItem("Exit"))Application::Get().Close();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::Begin("Settings");
            ImGui::Text("QuardCount: %d",NV::Renderer2D::GetStats().GetQuardCount());
            ImGui::Text("DrawCalls: %d",NV::Renderer2D::GetStats().GetDrawCalls());
            ImGui::Text("TotalVertexCount: %d",NV::Renderer2D::GetStats().GetTotalVertexCount());
            ImGui::Text("TotalIndexCount: %d",NV::Renderer2D::GetStats().GetTotalIndexCount());
            ImGui::Text("QuardIndexCount: %d",NV::Renderer2D::GetStats().GetQuardCount() * 6);
            ImGui::End();

            ImGui::Begin("FrameBuffer");
            ImVec2 size = ImGui::GetContentRegionAvail();
            /*
            why do this 
            because the ImGui::GetContentRegionAvail() will return the size of the window
            but we want the size of the frame buffer
            so we need to set the size of the frame buffer to the size of the window

            */
            NV_INFO("size: {0} {1}", size.x, size.y);
            if (size.x != m_ViewPortSize.x || size.y != m_ViewPortSize.y)
            {
                m_FrameBuffer->Resize(size.x, size.y);
                m_ViewPortSize = { size.x, size.y };
                m_OrthoCameraControl.OnResize(size.x, size.y);
            }
            uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
            ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(textureID)), ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });
            ImGui::End();

            ImGui::End();
        }
        else
        {
            ImGui::Begin("Settings");

            ImGui::ColorEdit4("SqareColor",glm::value_ptr(m_SqareColor));
            ImGui::Text("QuardCount: %d",NV::Renderer2D::GetStats().GetQuardCount());
            ImGui::Text("DrawCalls: %d",NV::Renderer2D::GetStats().GetDrawCalls());
            ImGui::Text("TotalVertexCount: %d",NV::Renderer2D::GetStats().GetTotalVertexCount());
            ImGui::Text("TotalIndexCount: %d",NV::Renderer2D::GetStats().GetTotalIndexCount());
            ImGui::Text("QuardIndexCount: %d",NV::Renderer2D::GetStats().GetQuardCount() * 6);

            uint32_t textureID = m_Texture2D->GetRendererID();
            ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(textureID)), ImVec2{ 1280, 720 });
            ImGui::End();
        }
    }

}
