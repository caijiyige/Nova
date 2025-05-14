#pragma once

#include "Novar/Renderer/GraphicsContext.h"

#include <glad.h>
#include <glfw3.h>
namespace NV
{
    class OpenGLContext: public  GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* window);
        void Init() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
}