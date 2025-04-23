#include "Nova.h"

namespace NV
{

    class  EditerLayer:public NV::Layer
    {

        public:
            EditerLayer();
            virtual ~EditerLayer() = default;

            virtual void OnAttach() override;
            virtual void OnDetach() override;

            void OnUpdate(Timestep ts) override;
            void OnEvent(Event& e) override;
            void OnImGuiRender() override;

        private:
        /* data */
            

            std::shared_ptr<Texture2D> m_Texture2D;
            std::shared_ptr<Texture2D> m_SpriteSheet;
            std::shared_ptr<SubTexture2D> m_TextureStairs, m_TextureStairs2;

            std::shared_ptr<FrameBuffer> m_FrameBuffer;

            NV::OrthographicCameraControl m_OrthoCameraControl;

            glm::vec3 m_CameraPosition;

            glm::vec4 m_SqareColor = {0.1f, 0.2f, 0.8f, 0.5f};


            std::unordered_map<char,std::shared_ptr<SubTexture2D>> m_TextureMap;
        
    };


}