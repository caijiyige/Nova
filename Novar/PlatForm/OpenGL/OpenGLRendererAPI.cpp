#include "OpenGLRendererAPI.h"

#include "glad.h"
namespace NV
{
    void OpenGLRendererAPI::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SOURCE0_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }
    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x,y,width,height);
    }
    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r,color.g,color.b,color.a);

    }
    void OpenGLRendererAPI::DrawIndexd(const std::shared_ptr<VertexArray> &vertexArray)
    {
        glDrawElements(GL_TRIANGLES,vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT,nullptr);

    }
}