#include "Nova.h" 
#include "OpenGLShader.h"
#include "OpenGLTexture2D.h"

#include "imgui.h"
#include "ext.hpp"

class ExampleLayer : public NV::Layer
{
public:
    ExampleLayer()
        : Layer("Example"),m_OrthoCameraControl(1280.0f/960.0f,true)
    {
        float sqarevertices[5*4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        m_SquareVA = NV::VertexArray::Create();
        std::shared_ptr<NV::VertexBuffer> sqareVB;
        sqareVB = NV::VertexBuffer::Create(sqarevertices,sizeof(sqarevertices));

        std::initializer_list<NV::BufferElement> elements = {
            {NV::ShaderDataType::Float3, "a_Position",false},
            {NV::ShaderDataType::Float2, "a_TexCoord",false}};

        NV::BufferLayout layout(elements);
           
        

        sqareVB->SetLayout(layout);

        m_SquareVA->AddVertexBuffer(sqareVB);


         unsigned int sqareindices[6] = { 0, 1, 2, 2, 3, 0};
         std::shared_ptr<NV::IndexBuffer> sqareIB ;
         sqareIB = NV::IndexBuffer::Create(sqareindices,sizeof(sqareindices)/sizeof(unsigned int));
       
        
        m_SquareVA->SetIndexBuffer(sqareIB);
        
        m_BlueShader = NV::Shader::Create("F:/LearnGameEngine/Nova/SandBox/assert/shaders/blue.glsl");
        //m_TextureShader = NV::Shader::Create("F:/LearnGameEngine/Nova/SandBox/assert/shaders/test.glsl");

        //auto texture = m_ShaderLibrary.Load("F:/LearnGameEngine/Nova/SandBox/assert/shaders/blue.glsl");
        auto texture = m_ShaderLibrary.Load("F:/LearnGameEngine/Nova/SandBox/assert/shaders/test.glsl");

        m_Texture2D = NV::Texture2D::Create("F:/LearnGameEngine/Nova/SandBox/assert/textures/Checkerboard.png");
        m_TextureCherno = NV::Texture2D::Create("F:/LearnGameEngine/Nova/SandBox/assert/textures/cherno.png");
        std::dynamic_pointer_cast<NV::OpenGLShader>(texture)->Bind();
        
        std::dynamic_pointer_cast<NV::OpenGLShader>(texture)->SetUniform1i("u_Texture",0);
        
    }

    void OnUpdate(NV::Timestep ts) override
    {
       // NV_INFO("ExampleLayer::Update");
        //NV_TRACE("Delte time :{0}",ts);
        m_OrthoCameraControl.OnUpdate(ts);

        
        NV::RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0});
        NV::RendererCommand::Clear();
        

        NV::Renderer::BeginScene(m_OrthoCameraControl.GetCamera());

        glm::mat4 scale = glm::scale(glm::mat4(1.0f),glm::vec3(0.1f));
        m_BlueShader->Bind();
        std::dynamic_pointer_cast<NV::OpenGLShader>(m_BlueShader)->SetUniform3f("u_Color",m_SqareColor);
        
        for (int y  = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                glm::vec3 pos ( x*0.2f, y*0.2f, 0.0f);
                glm::mat4 transform =glm::translate(glm::mat4(1.0f),pos) * scale;
                NV::Renderer::Submit(m_SquareVA,m_BlueShader,transform);
            }
        }

        auto testShader = m_ShaderLibrary.Get("test");

       
        m_Texture2D->Bind();
        NV::Renderer::Submit(m_SquareVA,testShader,glm::scale(glm::mat4(1.0f),glm::vec3(1.5f)));

        m_TextureCherno->Bind();
        NV::Renderer::Submit(m_SquareVA,testShader,glm::scale(glm::mat4(1.0f),glm::vec3(1.5f)));
        
        NV::Renderer::EndScene();
    }

     void OnEvent(NV::Event& e) override
     {
        m_OrthoCameraControl.OnEvent(e);
    
     }
     void OnImGuiRender() override
     {
        ImGui::Begin("settings");
        ImGui::ColorEdit3("SqareColor",glm::value_ptr(m_SqareColor));
        ImGui::End();

     }


    private:
        NV::ShaderLibrary m_ShaderLibrary;

        std::shared_ptr<NV::Shader> m_TextureShader;


        std::shared_ptr<NV::Shader> m_BlueShader;
        std::shared_ptr<NV::VertexArray> m_SquareVA;

        std::shared_ptr<NV::Texture2D> m_Texture2D, m_TextureCherno;

        NV::OrthographicCameraControl m_OrthoCameraControl;

        glm::vec3 m_CameraPosition;

        float m_CameraRotation = 0.0f;

        float m_CameraMoveSpeed = 0.1f;
        float m_CameraRotateSpeed = 2.0f;

        glm::vec3 m_SqarePosition;
        glm::vec3 m_SqareColor = {0.1f, 0.2f, 0.8f};


};