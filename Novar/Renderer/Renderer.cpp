#include "Renderer.h"

#include "OpenGLShader.h"

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

    void Renderer::BeginScene(OrthographicCamera &camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

    }
    void Renderer::EndScene()
    {


    }
    void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray,const std::shared_ptr<Shader>& shader,const glm::mat4& transform)
    {

        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjection",m_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ModelMatrix",transform);

        vertexArray->Bind();
       RendererCommand::DrawIndexed(vertexArray);
    }
}