#include "Novar/Imgui/ImguiLayer.h"

//IMGUI
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Novar/Core/Application.h"

#include <glad.h>
#include <glfw3.h> // Include glfw3.h after our OpenGL definitions


namespace NV
{
    ImguiLayer::ImguiLayer()
        : Layer("ImguiLayer")
    {
    }
    ImguiLayer::~ImguiLayer()
    {
    }
    void ImguiLayer::OnAttach()
    {
        //NV_PROFILE_FUNCTION();
        //NV_INFO("ImguiLayer::OnAttach");
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Viewports
        //io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen; // Enable Touch Screen
        //io.ConfigFlags |= ImGuiConfigFlags_NoMouse; // Disable Mouse
        //io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; // Disable Mouse Cursor Change

        io.Fonts->AddFontFromFileTTF("F:/LearnGameEngine/Nova/Novar/assert/fonts/opensans/OpenSans-Light.ttf", 16.0f);

        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f; // Set the alpha of the window background to 1.0f
        }

        Application& app = Application::Get();
        GLFWwindow* window = (GLFWwindow*)app.GetWindow().GetNativeWindow();
        // Setup Platform/Renderer bindings

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImguiLayer::OnDetach()
    {
        
        //NV_PROFILE_FUNCTION();
        //NV_INFO("ImguiLayer::OnDetach");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }


    void ImguiLayer::OnUpdate(Timestep ts)
    {
        
       
    }

    void ImguiLayer::OnEvent(Event &e)
    {
       e.m_Handled = m_BlockEvent;
        
    }

    void ImguiLayer::Begin()
    {
        //NV_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
    }

    void ImguiLayer::End()
    {
        //NV_PROFILE_FUNCTION();
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
       
    }

}