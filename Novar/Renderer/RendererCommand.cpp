#include "RendererCommand.h"

#include "OpenGLRendererAPI.h"


namespace NV
{
    RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;
}