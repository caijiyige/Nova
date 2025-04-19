#pragma once

#include "Window.h"
#include "Core.h"
#include "glad.h"
#include "glfw3.h"
#include "GraphicsContext.h"

namespace NV
{
    class WindowsWindow : public Window
    {
        public:
            WindowsWindow(const WindowProps& props);
            virtual ~WindowsWindow();

            void OnUpdate() override;

            unsigned int GetWidth() const override { return m_Data.Width; }
            unsigned int GetHeight() const override { return m_Data.Height; }

            inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
            void SetVSync(bool enabled) override;
            bool IsVSync() const override;

            inline virtual void* GetNativeWindow() const { return m_Window; }
            //static void GLFWErrorCallback(int error, const char* description);
        private:
            virtual void Init(const WindowProps& props);
            virtual void Shutdown();

            GLFWwindow* m_Window;
            //GLFWwindow* m_WindowHandle;
            GraphicsContext* m_Context;
            struct WindowData
            {
                std::string Title;
                unsigned int Width, Height;
                bool VSync;

                EventCallbackFn EventCallback;
            };

            WindowData m_Data;
    };
}
