#include "Texture.h"
#include "Renderer.h"

#include "OpenGLTexture2D.h"



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