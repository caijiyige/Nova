#include "Buffer.h"
#include "Core.h"
#include "Renderer.h"

#include "OpenGLBuffer.h"
namespace NV{

    VertexBuffer* VertexBuffer::Create(float *vertices, unsigned int size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                NV_CORE_ASSERT(false,"RendererAPI::None is currently not supported!");
                return nullptr;
                
            case RendererAPI::API::OpenGL:
                return new OpenGLVertexBuffer(vertices,size);
        }
        
    }

    IndexBuffer *IndexBuffer::Create(unsigned int* indices, unsigned int size)
    {

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                NV_CORE_ASSERT(false,"RendererAPI::None is currently not supported!");
                return nullptr;
                
            case RendererAPI::API::OpenGL:
                return new OpenGLIndexBuffer(indices,size);
        }
    }

}
