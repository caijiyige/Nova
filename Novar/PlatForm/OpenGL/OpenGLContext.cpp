#include "OpenGLContext.h"
#include "Log.h"



namespace NV
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        NV_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        NV_CORE_ASSERT(status, "Failed to initialize Glad!");

       // NV_CORE_INFO("OpenGL Info:");
        NV_CORE_INFO("  Vendor: {0}", (void*)glGetString(GL_VENDOR));
        NV_CORE_INFO("  Renderer: {0}", (void*)glGetString(GL_RENDERER));
        NV_CORE_INFO("  Version: {0}", (void*)glGetString(GL_VERSION));
        NV_CORE_INFO("  GLSL Version: {0}", (void*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}

