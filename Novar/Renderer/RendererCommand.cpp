#include "Novar/Renderer/RendererCommand.h"

#include "Novar/Platform/OpenGL/OpenGLRendererAPI.h"


namespace NV
{
    RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;
}