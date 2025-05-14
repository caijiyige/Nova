#include "Novar/Renderer/Renderer.h"

#include "Novar/PlatForm/OpenGL/OpenGLShader.h"


namespace NV{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::Init()
    {
        RendererCommand::Init();
    }

    void Renderer::OnWindowResize(float width, float height)
    {
        RendererCommand::SetViewport(0,0,width,height);
    }



    void Renderer::BeginScene(const OrthographicCamera &camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

    }
    void Renderer::EndScene()
    {


    }
}