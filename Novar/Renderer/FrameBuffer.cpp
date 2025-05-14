#include "Novar/PCH.h"
#include "Novar/Core/Base.h"
#include "Novar/Renderer/FrameBuffer.h"
#include "Novar/Renderer/Renderer.h"

#include "Novar/PlatForm/OpenGL/OpenGLFrameBuffer.h"


namespace NV
{
    std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
    {
         switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                NV_CORE_ASSERT(false,"RendererAPI::None is currently not supported!");
                return nullptr;
                
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLFrameBuffer>(spec);
        }
        return nullptr;
        
    }
}