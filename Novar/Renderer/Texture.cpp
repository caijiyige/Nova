#include "Novar/Renderer/Texture.h"
#include "Novar/Renderer/Renderer.h"

#include "Novar/PlatForm/OpenGL/OpenGLTexture2D.h"
#include "Novar/Core/Base.h"
namespace NV
{
    std::shared_ptr<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height)
    {
     switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                NV_CORE_ASSERT(false,"RendererAPI::None is currently not supported!");
                return nullptr;
                
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLTexture2D>(width,height);
        }
        return nullptr;
    }

    std::shared_ptr<Texture2D> Texture2D::Create(const std::string &path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                NV_CORE_ASSERT(false,"RendererAPI::None is currently not supported!");
                return nullptr;
                
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLTexture2D>(path);
        }
        return nullptr;
    }
}