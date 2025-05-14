#include "Novar/Renderer/VertexArray.h"
#include "Novar/Renderer/Renderer.h"

#include "Novar/PlatForm/OpenGL/OpenGLVertexArray.h"
#include "Novar/Core/Base.h"

namespace NV
{
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                NV_CORE_ASSERT(false,"RendererAPI::None is currently not supported!");
                return nullptr;
                
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexArray>();
        }
        return nullptr;
    }
}