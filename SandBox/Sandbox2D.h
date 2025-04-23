#include "Nova.h"

class  Sandbox2D:public NV::Layer
{

    public:
        Sandbox2D();
        virtual ~Sandbox2D() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void OnUpdate(NV::Timestep ts) override;
        void OnEvent(NV::Event& e) override;
        void OnImGuiRender() override;

    private:
    /* data */
        

        std::shared_ptr<NV::Texture2D> m_Texture2D;
        std::shared_ptr<NV::Texture2D> m_SpriteSheet;
        std::shared_ptr<NV::SubTexture2D> m_TextureStairs, m_TextureStairs2;

        

        NV::OrthographicCameraControl m_OrthoCameraControl;

        glm::vec3 m_CameraPosition;

        glm::vec4 m_SqareColor = {0.1f, 0.2f, 0.8f, 0.5f};


        std::unordered_map<char,std::shared_ptr<NV::SubTexture2D>> m_TextureMap;
    
};


