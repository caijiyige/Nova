#include "Novar/Nova.h"
#include "imgui.h"
#include "Novar/Renderer/EditorCamera.h"
#include <filesystem>
namespace NV
{
    enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2, Pause = 3
		};
		
    class  EditorLayer : public Layer
    {

        public:
            EditorLayer();
            virtual ~EditorLayer() = default;

            virtual void OnAttach() override;
            virtual void OnDetach() override;

            void OnUpdate(Timestep ts) override;
            void OnEvent(Event& e) override;
            void OnImGuiRender() override;
        private:

            bool OnKeyPressed(KeyPressedEvent& e);
            bool OnMousePressed(MouseButtonPressedEvent& e);

            void NewScene();
            //void OpenScene();
            void OpenScene(const std::filesystem::path& path);
            //void SaveSceneAs();
            void SaveScene();

            void OpenProject();
            //void OpenProject(const std::filesystem::path& path);
            //void SaveProject();

            void OnScenePlay();
            void OnSceneSimulate();
            void OnSceneStop();
            void OnScenePause();

            void UIToolbar();
            void OnDuplicateEntity();
            void OnOverlayRender();


        private:
        
            std::shared_ptr<Texture2D> m_Texture2D;
            std::shared_ptr<Texture2D> m_SpriteSheet;
            std::shared_ptr<Texture2D> m_IconPlay;
            std::shared_ptr<Texture2D> m_IcomSimulate;
            std::shared_ptr<Texture2D> m_IconStop;
            std::shared_ptr<Texture2D> m_IconPause;
            std::shared_ptr<Texture2D> m_IconStep;
            std::shared_ptr<SubTexture2D> m_TextureStairs, m_TextureStairs2;

             glm::vec4 m_SpriteColor = {0.1f, 0.2f, 0.8f, 0.5f};

            std::shared_ptr<FrameBuffer> m_FrameBuffer;


            std::shared_ptr<Scene> m_ActiveScene;
            std::shared_ptr<Scene> m_EditorScene;
            std::filesystem::path m_EditorScenePath;

            std::shared_ptr<Entity> m_SquareEntity;
            std::shared_ptr<Entity> m_CameraEntity;
            std::shared_ptr<Entity> m_HoveredEntity;


            std::shared_ptr<EditorCamera> m_EditorCamera;
            
            std::shared_ptr<SceneHierachyPanel> m_SceneHierarchyPanel;
            std::shared_ptr<SceneSerializer> m_SceneSerializer;

            SceneState m_SceneState = SceneState::Edit;

            bool m_bShowDemoWindow = true;
            bool m_bShowPhysicsCollider = false;

            //Resize data
            bool m_bViewportHovered;
            bool m_bViewportFocused;
            ImVec2 m_vec2RenderViewPortSize;
            std::array<ImVec2, 2> m_vec2RenderViewPortBounds;
    };


}