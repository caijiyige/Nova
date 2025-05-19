#include "Novar/Renderer/RendererAPI.h"
#include "Novar/PlatForm/OpenGL/OpenGLRendererAPI.h"
#include "Novar/Core/Base.h"
namespace NV{

    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    NV_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLRendererAPI>();
		}

		NV_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}