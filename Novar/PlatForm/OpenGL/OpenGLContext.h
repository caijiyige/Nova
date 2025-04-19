#pragma once

#include "GraphicsContext.h"
#include "glad.h" // Include glad to get all the required OpenGL headers
#include "glfw3.h"

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