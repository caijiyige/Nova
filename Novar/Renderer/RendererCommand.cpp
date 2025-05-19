#include "Novar/Renderer/RendererCommand.h"

#include "Novar/Platform/OpenGL/OpenGLRendererAPI.h"


namespace NV
{
    std::unique_ptr<RendererAPI> RendererCommand::s_RendererAPI = RendererAPI::Create();
}