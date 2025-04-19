
#include "VertexArray.h"
#include "Renderer.h"

#include "OpenGLVertexArray.h"
namespace NV
{
    VertexArray *VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                NV_CORE_ASSERT(false,"RendererAPI::None is currently not supported!");
                return nullptr;
                
            case RendererAPI::API::OpenGL:
                return new OpenGLVertexArray();
        }
        return nullptr;
    }
}