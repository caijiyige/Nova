
#include "Sandbox2D.h"
#include "Instrumentor.h"
#include "imgui.h"
#include "ext.hpp"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"),m_OrthoCameraControl(1280.0f/960.0f,true)
    {
    }
void Sandbox2D::OnAttach()
{
    m_Texture2D = NV::Texture2D::Create("F:/LearnGameEngine/Nova/SandBox/assert/textures/Checkerboard.png");

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(NV::Timestep ts)
{
        NV_PROFILE_FUNCTION();
        
        {
            NV_PROFILE_SCOPE("m_OrthoCameraControl::OnUpdate");
            m_OrthoCameraControl.OnUpdate(ts);
        }


        NV::Renderer2D::ResetStats();
        
        NV::RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0});
        NV::RendererCommand::Clear();
        NV::RendererCommand::Init();
        {
            NV_PROFILE_SCOPE("Renderer2D::DrawQuard");
            
            NV::Renderer2D::BeginScene(m_OrthoCameraControl.GetCamera());
            NV::Renderer2D::DrawQuard({1.0f,1.0f},{1.0f,1.0f},{0.1f, 0.2f, 0.8f, 1.0f});
            
            //NV::Renderer2D::DrawRotationQuard({-0.5f,-0.5f},{0.5f,0.5f},glm::radians(45.0f),{0.8f, 0.2f, 0.1f, 1.0f});
            NV::Renderer2D::DrawQuard({0.0f,0.0f,-0.1f},{10.0f,10.0f},m_Texture2D,10.0f);
            //NV::Renderer2D::DrawRotationQuard({0.0f,0.0f,-0.1f},{10.0f,10.0f},45.0f,m_Texture2D,10.0f,{0.8f,0.5f,0.3f,0.5f});
            NV::Renderer2D::EndScene();

            NV::Renderer2D::BeginScene(m_OrthoCameraControl.GetCamera());

            for(float y = -5.0f; y<5.0f; y+=0.1f)
            {
                for(float x = -5.0f; x<5.0f; x+=0.1f)
                {
                    glm::vec4 color = {x/5.0f,y/5.0f,0.0f,0.5f};
                    NV::Renderer2D::DrawQuard({x,y,-0.2f},{0.45f,0.45f},color);
                }
            }
            
            NV::Renderer2D::EndScene();
           
        }
        
}

void Sandbox2D::OnEvent(NV::Event &e)
{
    m_OrthoCameraControl.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
    
    NV_PROFILE_FUNCTION();
    NV_PROFILE_SCOPE("Sandbox2D::OnImGuiRender");
    ImGui::Begin("settings");
    ImGui::ColorEdit4("SqareColor",glm::value_ptr(m_SqareColor));
    ImGui::Text("QuardCount: %d",NV::Renderer2D::GetStats().GetQuardCount());
    ImGui::Text("DrawCalls: %d",NV::Renderer2D::GetStats().GetDrawCalls());
    ImGui::Text("TotalVertexCount: %d",NV::Renderer2D::GetStats().GetTotalVertexCount());
    ImGui::Text("TotalIndexCount: %d",NV::Renderer2D::GetStats().GetTotalIndexCount());
    ImGui::Text("QuardIndexCount: %d",NV::Renderer2D::GetStats().GetQuardCount() * 6);
    ImGui::End();
}
