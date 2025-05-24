#include "NovaEditor/EditorLayer.h"

#include <glm/ext.hpp>
namespace NV
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer"),m_SceneState(SceneState::Edit)
    {
        Renderer2D::Init();
        //创建编辑器相机
        m_EditorCamera = std::make_shared<EditorCamera>(45.0f, 1280.0f / 720.0f,-1.0f,1.0f);
        //创建编辑器场景
        m_EditorScene = std::make_shared<Scene>();
        //创建场景层级面板
        m_SceneHierarchyPanel = std::make_shared<SceneHierachyPanel>(m_EditorScene);

        //创建场景序列化器
        m_SceneSerializer = std::make_shared<SceneSerializer>(m_EditorScene);

        //创建帧缓冲
        //纹理规范数组
        // std::vector<FrameBufferTextureSpecification> vecFrameBufferTextureSpecification =
        // {
        //     FrameBufferTextureFormat::RGBA8,
        //     FrameBufferTextureFormat::RED_INTEGER,
        //     FrameBufferTextureFormat::DEPTH24STENCIL8
        // };

        //附件规范 列表初始化 
        FrameBufferAttachmentSpecification frameBufferAttachmentSpecification{FrameBufferTextureFormat::RGBA8,
            FrameBufferTextureFormat::RED_INTEGER,
            FrameBufferTextureFormat::DEPTH24STENCIL8};
        
        //帧缓冲规范
        NV::FrameBufferSpecification spec;
        spec.Width = 1280;
        spec.Height = 720;
        spec.AttachmentSpec = frameBufferAttachmentSpecification;

        m_FrameBuffer = FrameBuffer::Create(spec);

        //创建纹理
        
        m_IconPause = Texture2D::Create("F:/LearnGameEngine/Nova/Novar/assert/icons/PauseButton.png");
        m_IconPlay = Texture2D::Create("F:/LearnGameEngine/Nova/Novar/assert/icons/PlayButton.png");
        m_IconStep = Texture2D::Create("F:/LearnGameEngine/Nova/Novar/assert/icons/StepButton.png");
        m_IconStop = Texture2D::Create("F:/LearnGameEngine/Nova/Novar/assert/icons/StopButton.png");
        m_IcomSimulate = Texture2D::Create("F:/LearnGameEngine/Nova/Novar/assert/icons/SimulateButton.png");
        
    }
    void EditorLayer::OnAttach()
    {
        /*
        auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenProject(projectFilePath);
		}
		else
		{
			// TODO(Yan): prompt the user to select a directory
			// NewProject();

			// If no project is opened, close Hazelnut
			// NOTE: this is while we don't have a new project path
			//if (!OpenProject())
				//Application::Get().Close();
            OpenProject();


		}*/

    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(NV::Timestep ts)
    {
            NV_PROFILE_FUNCTION();

            //Resize
            {
                //Scene Resize

                //frame buffer Resize

                //camera Resize
            }
            
            //Prepare Renderer
            
            Renderer2D::ResetStats();
            m_FrameBuffer->Bind();
            RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0});
            RendererCommand::Clear();

            m_FrameBuffer->ClearAttachment(1, -1);
            
            m_FrameBuffer->ReadPixelColor();
            //Scene State

            switch (m_SceneState)
	        {
                case SceneState::Play:
                {
                    m_ActiveScene->OnUpdateRuntime(ts);
                    break;
                }
                case SceneState::Edit:
                {
                    m_EditorCamera->OnUpdate(ts);
                    m_EditorScene->OnUpdateEditor(ts,m_EditorCamera);
                    break;
                }
                case SceneState::Simulate:
                {
                    m_ActiveScene->OnUpdateRuntime(ts);
                    break;
                }
	        }

            //mouse pick

            {
                auto[mx, my] = ImGui::GetMousePos();
                mx -= m_vec2RenderViewPortBounds[0].x;
                my -= m_vec2RenderViewPortBounds[0].y;
                my = m_vec2RenderViewPortSize.y - my;
                int mouseX = (int)mx;
                int mouseY = (int)my;

                if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)m_vec2RenderViewPortSize.x && mouseY < (int)m_vec2RenderViewPortSize.y)
                {
                    int pixelData = m_FrameBuffer->ReadPixelInt(1, mouseX, mouseY);
                    NV_INFO("mousex {0}  {1} pixelData {2}",mouseX, mouseY,pixelData);
                    //m_HoveredEntity = pixelData == -1 ? Entity() : Entity(m_ActiveScene->GetRegistry(), (entt::entity)pixelData);
                }

            }

            
            //状态切换
            OnOverlayRender();
            
            m_FrameBuffer->UnBind();      
    }

    void EditorLayer::OnEvent(Event &e)
    {
        //m_OrthoCameraControl.OnEvent(e);

        if(m_SceneState == SceneState::Edit)
        {
            auto spEditorCamera = std::dynamic_pointer_cast<EditorCamera>(m_EditorCamera);
            spEditorCamera->OnEvent(e);
        }
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(NV_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(NV_BIND_EVENT_FN(EditorLayer::OnMousePressed));
    }

    void EditorLayer::OnImGuiRender()
    {
        NV_PROFILE_FUNCTION();

        // Note: Switch this to true to enable dockspace
        static bool dockingEnabled = true;

        static bool opt_padding = false;
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

        if(!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        {
            if (!opt_padding)
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
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
					OpenScene();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();
				
				ImGui::EndMenu();
			}
                    
                ImGui::EndMenuBar();
            }
            
            {
                m_SceneHierarchyPanel->OnImGuiRender(); 
                //m_ContentBrowserPanel->OnImGuiRender();
            }

            {
                ImGui::Begin("Status");
                ImGui::Text("QuardCount: %d",NV::Renderer2D::GetStats().GetQuadCount());
                ImGui::Text("DrawCalls: %d",NV::Renderer2D::GetStats().GetDrawCalls());
                ImGui::Text("TotalVertexCount: %d",NV::Renderer2D::GetStats().GetTotalVertexCount());
                ImGui::Text("TotalIndexCount: %d",NV::Renderer2D::GetStats().GetTotalIndexCount());
                ImGui::Text("QuardIndexCount: %d",NV::Renderer2D::GetStats().GetQuadCount() * 6);
                ImGui::End();
            }
            {
                ImGui::Begin("Setting");
                ImGui::Checkbox("Show Physics Collider", &m_bShowPhysicsCollider);
                ImGui::Checkbox("Show Demo Window", &m_bShowDemoWindow);
                if (m_bShowDemoWindow)
                {
                    ImGui::ShowDemoWindow(&m_bShowDemoWindow);
                }
                ImGui::End();
            }
            
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::Begin("viewport");

                ImVec2 vec2MinRegion = ImGui::GetWindowContentRegionMin();
                ImVec2 vec2MaxRegion = ImGui::GetWindowContentRegionMax();
                auto vec2WindowPos = ImGui::GetWindowPos();
                m_vec2RenderViewPortBounds[0] = { vec2MinRegion.x + vec2WindowPos.x, vec2MinRegion.y + vec2WindowPos.y };
                m_vec2RenderViewPortBounds[1] = { vec2MaxRegion.x + vec2WindowPos.x, vec2MaxRegion.y + vec2WindowPos.y };

                m_bViewportHovered = ImGui::IsWindowHovered();
                m_bViewportFocused = ImGui::IsWindowFocused();
                Application::Get().BlockEvents(!m_bViewportHovered);

                m_vec2RenderViewPortSize = ImGui::GetContentRegionAvail();
                auto spFrameBuffer = std::dynamic_pointer_cast<FrameBuffer>(m_FrameBuffer);
                NV_ASSERT(spFrameBuffer, "FrameBuffer is null in Edit Layer");
                auto uiTextureID = spFrameBuffer->GetColorAttachmentRendererID();
		        //ImGui::Image(reinterpret_cast<void*>(uiTextureID),  m_vec2RenderViewPortSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

                ImGui::Image((ImTextureID)(intptr_t)uiTextureID, m_vec2RenderViewPortSize, ImVec2(0, 1), ImVec2(1, 0));

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                    {
                        const wchar_t* path = (const wchar_t*)payload->Data;
                        OpenScene(path);
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::End();
            }
            ImGui::PopStyleVar();
            

            UIToolbar();
        }
        ImGui::End();
    }
    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.IsRepeat())
        {
            return false;
        }

        bool bControl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool bShift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        switch (e.GetKeyCode())
        {
        case Key::N:
        {
            if (bControl)
            {
                NewScene();
            }
            break;
        }
        case Key::O:
        {
            if (bControl)
            {
                OpenScene();
            }
            break;
        }
        case Key::S:
        {
            if (bControl)
            {
                bShift ? SaveSceneAs() : SaveScene();
            }
            break;
        }
        case Key::D:
        {
            if (bControl)
            {
                OnDuplicateEntity();
            }
            break;
        }
        /*
        case Key::Q:
        {
            m_iGizmoType = -1;
            break;
        }
        case Key::W:
        {
            m_iGizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        }
        case Key::E:
        {
            m_iGizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        }
        case Key::R:
        {
            m_iGizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        }*/
        default:
            break;
        }
        return true;
    }

    bool EditorLayer::OnMousePressed(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == Mouse::ButtonLeft && m_bViewportHovered
            /*&& !ImGuizmo::IsOver() */&& !Input::IsKeyPressed(Key::LeftAlt))
        {
            m_SceneHierarchyPanel->SetSelectedEntity(m_HoveredEntity);
        }

        return false;
    }
    void EditorLayer::NewScene()
    {
        m_EditorScene = std::make_shared<Scene>();
        m_EditorScene->OnViewportResize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);
        m_SceneHierarchyPanel->SetContext(m_EditorScene);

        m_EditorScenePath = std::filesystem::path();
    }

    void EditorLayer::OpenScene()
    {
        auto sFilePath = FileDialog::OpenFile("Nova Scene (*.scene)\0*.scene\0");
        if (!sFilePath.empty())
        {
            OpenScene(sFilePath);
        }
    }

    void EditorLayer::OpenScene(const std::filesystem::path& path)
    {
        if (m_SceneState != SceneState::Edit)
        {
            OnSceneStop();
        }

        if (path.extension().string()!=".scene")
        {
            NV_WARN("Could not load {0} - not a scene file", path.filename().string());
            return;
        }

        NewScene();
        m_SceneSerializer->SetSelectedScene(m_EditorScene);
        m_SceneSerializer->Deserialize(path.string());
        m_EditorScenePath = path;
    }

   void EditorLayer::SaveSceneAs()
        {
            auto sFilePath = FileDialog::SaveFile("Nova Scene (*.scene)\0*.scene\0");
            if (!sFilePath.empty())
            {
                m_SceneSerializer->Serialize(sFilePath);
                m_EditorScenePath = sFilePath;
            }
        }

    void EditorLayer::SaveScene()
    {
        if (!m_EditorScenePath.empty())
        {
            m_SceneSerializer->Serialize(m_EditorScenePath.string());
        }
    }

    void EditorLayer::OpenProject()
    {
        std::string filepath = FileDialog::OpenFile("Hazel Project (*.hproj)\0*.hproj\0");
        OpenProject(filepath);
    }

    void EditorLayer::OpenProject(const std::filesystem::path& path)
    {
        /*if (Project::Load(path))
        {
            //ScriptEngine::Init();

            auto startScenePath = Project::GetAssetFileSystemPath(Project::GetActive()->GetConfig().StartScene);
			OpenScene(startScenePath);
			m_ContentBrowserPanel = std::make_shared<ContentBrowserPanel>();
        }*/
       m_ContentBrowserPanel = std::make_shared<ContentBrowserPanel>();
    }

    void EditorLayer::SaveProject()
    {
        //Project::SaveActive();
    }

    void EditorLayer::OnScenePlay()
    {
        m_SceneState = SceneState::Play;

        m_ActiveScene = std::make_shared<Scene>(m_EditorScene);
        m_ActiveScene->OnRuntimeStart();

        m_SceneHierarchyPanel->SetContext(m_ActiveScene);
    }

    void EditorLayer::OnSceneSimulate()
    {
        m_SceneState = SceneState::Simulate;
        m_ActiveScene = std::make_shared<Scene>(m_EditorScene);
        m_ActiveScene->OnSimulationStart();
    }

    void EditorLayer::OnSceneStop()
    {
        switch (m_SceneState)
        {
            case SceneState::Play:
            {
                m_ActiveScene->OnRuntimeStop();
                break;
            }
            case SceneState::Simulate:
            {
                m_ActiveScene->OnSimulationStop();
                break;
            }
        }
        m_SceneState = SceneState::Edit;

    }

    void EditorLayer::OnScenePause()
    {
        if (m_SceneState == SceneState::Edit)
        {
            return;
        }
        m_SceneState = SceneState::Pause;
        m_ActiveScene->SetPaused(true);
    }

    void EditorLayer::OnDuplicateEntity()
    {
        if (m_SceneState == SceneState::Edit)
        {
            auto spEntity = m_SceneHierarchyPanel->GetSelectedEntity();
            m_EditorScene->CreateEntity(spEntity);
        }
    }
    // void EditorLayer::OnDuplicateEntity()
    // {
    //     if (m_SceneHierachyPanel.GetSelectedEntity())
    //     {
    //         Entity entity = m_SceneHierachyPanel.GetSelectedEntity();
    //         Entity newEntity = m_Scene->DuplicateEntity(entity);
    //         m_SceneHierachyPanel.SetSelectedEntity(newEntity);
    //     }
    // }

    void EditorLayer::OnOverlayRender()
    {
        if (!m_bShowPhysicsCollider)
        {
            return;
        }

        switch (m_SceneState)
        {
            case SceneState::Play:
            {
                if (m_ActiveScene != nullptr)
                {
                    auto spCameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
                    auto spCamera = spCameraEntity->GetComponent<CameraComponent>();
                    auto cam = std::make_shared<Camera>();
                    cam.reset(&spCamera.Camera);
                    m_ActiveScene->OnShowPhysicsCollider(cam);
                }
                break;
            }
            case SceneState::Edit:
            {
                if (m_EditorScene != nullptr)
                {
                    m_EditorScene->OnShowPhysicsCollider(m_EditorCamera);
                }
                break;
            }
        }
    }


    void EditorLayer::UIToolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

        ImGui::Begin("#toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImVec4 tintColor = ImVec4(1, 1, 1, 1);

        float size = ImGui::GetWindowHeight() - 4.0f;
        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - size * 0.5f);


        bool bPlay = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
        bool bSimulate = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
        bool bPause = m_SceneState != SceneState::Edit;

        if(bPlay)
        {
            auto spIconTexture = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
                ? m_IconPlay : m_IconStop;
            if (ImGui::ImageButton("PlayButton", (ImTextureID)(spIconTexture->GetRendererID()), ImVec2(size, size),
                ImVec2(0, 0), ImVec2(1, 1),  ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor))
            {
                switch (m_SceneState)
                {
                case SceneState::Play:
                {
                    OnSceneStop();
                    break;
                }
                case SceneState::Edit:
                case SceneState::Simulate:
                {
                    OnScenePlay();
                    break;
                }
                default:
                    break;
                }
            }
        }

        if (bSimulate)
        {
            if (bPlay)
            {
                ImGui::SameLine();
            }
            {
                auto spIconTexture = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
                    ? m_IcomSimulate : m_IconStop;
                if (ImGui::ImageButton("SimulateButton", (ImTextureID)(spIconTexture->GetRendererID()), ImVec2(size, size),
                    ImVec2(0, 0), ImVec2(1, 1),  ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor))
                {
                    switch (m_SceneState)
                    {
                    case SceneState::Simulate:
                    {
                        OnSceneStop();
                        break;
                    }
                    case SceneState::Edit:
                    case SceneState::Play:
                    {
                        OnSceneSimulate();
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
        }

       if (bPause)
        {
            bool bIsPaused = m_ActiveScene->IsPaused();
            ImGui::SameLine();
            {
                if (ImGui::ImageButton("",(ImTextureID)(m_IconPause->GetRendererID()), ImVec2(size, size),
                    ImVec2(0, 0), ImVec2(1, 1),  ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor))
                {
                    m_ActiveScene->SetPaused(!bIsPaused);
                }
            }
            if (bIsPaused)
            {
                ImGui::SameLine();
                {
                    if (ImGui::ImageButton("",(ImTextureID)(m_IconStep->GetRendererID()), ImVec2(size, size),
                        ImVec2(0, 0), ImVec2(1, 1),  ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor))
                    {
                        m_ActiveScene->Step();
                    }
                }
            }
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::End();
    }
}
