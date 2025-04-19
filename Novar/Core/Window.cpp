#include "Window.h"

#include "WindowsWindow.h"

namespace NV{
    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    } 
}
